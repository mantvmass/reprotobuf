# REPROTOBUF
Reprotobuf is a toolkit for decoding data serialized using Protocol Buffers (protobuf).  
> ทำไมถึงใช้ `C++` ในการพัฒนาโปรเจคนี้: คือไม่ได้มีอะไรมากแค่อยากลองฝึกเขียน `C++` แบบจริงๆจังๆเฉยๆ ถ้าจะไปฝึกเขียนตามแบบฝึกหัดต่างๆมันหน้าเบื่ออะ

## Compile
- Visual Studio Community 2022 Release - 17.3.6
- Cmake 3.26.4
- Windows 10 SDK version 10.0.19041.0
- GNU Make 4.4.1

Use the following command to install all dependencies listed in `vcpkg.json`

```bash
vcpkg install --triplet x86-windows
```

#### For Release
```bash
make release
```

#### For Debug
```bash
make debug
```

## หลักการสำคัญในการ Decode Protobuf

1. **Protobuf ใช้ Tag และ Wire Type**  
   ข้อมูลใน Protobuf ถูกจัดเก็บเป็นคู่ของ **Tag** (field number) และ **Wire Type** ซึ่งเป็นรูปแบบการเข้ารหัสสำหรับค่าของ field นั้น  

   1.1 **Wire Type มี 6 แบบ**  
   - **0**: Varint (จำนวนเต็มที่ใช้ variable-length encoding เช่น `int32`, `int64`, `uint32`, `bool`)  
   - **1**: Fixed64 (จำนวนเต็ม 64 บิตแบบคงที่ เช่น `double`, `fixed64`, `sfixed64`)  
   - **2**: Length-delimited (ข้อมูลที่มีความยาวระบุ เช่น `string`, `bytes`, `embedded messages`, `packed repeated fields`)  
   - **3 & 4**: Start/End group (deprecated)  
   - **5**: Fixed32 (จำนวนเต็ม 32 บิตแบบคงที่ เช่น `float`, `fixed32`, `sfixed32`)  

2. **โครงสร้างของ Field ในข้อมูล Protobuf**  
   - แต่ละ Field ประกอบด้วย **Key** (รวม Tag และ Wire Type) ตามด้วย **Value**
   - Key เป็นตัวเลข `Varint` ที่เข้ารหัสโดย:  
     ```
     Key = (Tag << 3) | Wire Type
     ```
   - Value จะมีรูปแบบตาม Wire Type ที่กำหนด

3. **รูปแบบการจัดเก็บข้อมูล**  
   - ข้อมูล Protobuf เป็น Binary format ที่ออกแบบมาเพื่อประสิทธิภาพในการเข้ารหัสและถอดรหัส
   - ข้อมูลถูกจัดเก็บแบบต่อเนื่องไม่มี delimiter ระหว่าง field
   - ต้องอ่านและแปลความหมายตาม Wire Type อย่างถูกต้องเพื่อแยกแต่ละ field

4. **การ Decode ข้อมูลโดยไม่มี `.proto` file**  
   - เมื่อไม่มี `.proto` file จะไม่ทราบชื่อ field และ data type ที่แน่นอน
   - สามารถอ่านโครงสร้างข้อมูลได้จาก Tag และ Wire Type
   - ต้องวิเคราะห์รูปแบบข้อมูลเพื่อเดาความหมายของแต่ละ field

## ขั้นตอนในการ Decode Protobuf

1. **อ่าน Key (Tag + Wire Type)**  
   - อ่าน varint แรกเพื่อรับค่า key
   - แยกส่วนประกอบ:
     ```
     Wire Type = Key & 0x07  (3 bits สุดท้าย)
     Tag = Key >> 3         (shift ขวา 3 bits)
     ```

2. **อ่านค่าตาม Wire Type**  
   สำหรับแต่ละ Wire Type:
   - **0 (Varint)**:
     - อ่านทีละไบต์จนกว่าจะเจอไบต์ที่มี MSB = 0
     - รวมค่า 7 bits ของแต่ละไบต์เข้าด้วยกัน
   - **1 (Fixed64)**:
     - อ่านข้อมูลจำนวน 8 ไบต์
   - **2 (Length-delimited)**:
     - อ่านความยาวเป็น varint
     - อ่านข้อมูลตามความยาวที่ระบุ
   - **5 (Fixed32)**:
     - อ่านข้อมูลจำนวน 4 ไบต์

## ตัวอย่างการ Decode ด้วยมือ

ข้อมูลตัวอย่าง (Binary): `08 96 01 12 07 74 65 73 74 69 6E 67 1A 05 68 65 6C 6C 6F`

#### ขั้นตอนการ Decode:

1. **Field ที่ 1**:
   - `08`:
     - Binary = `00001000`
     - Tag = `1` (`00001`), Wire Type = `0` (`000`)
   - `96 01`:
     - Varint value = 150 (0x96 0x01 ในรูปแบบ varint)

2. **Field ที่ 2**:
   - `12`:
     - Binary = `00010010`
     - Tag = `2` (`00010`), Wire Type = `2` (`010`)
   - `07`: ความยาว 7 ไบต์
   - `74 65 73 74 69 6E 67`:
     - ASCII: "testing"

3. **Field ที่ 3**:
   - `1A`:
     - Binary = `00011010`
     - Tag = `3` (`00011`), Wire Type = `2` (`010`)
   - `05`: ความยาว 5 ไบต์
   - `68 65 6C 6C 6F`:
     - ASCII: "hello"

#### ผลลัพธ์หลัง Decode:
```json
{
  "1": 150,
  "2": "testing",
  "3": "hello"
}
```

## ตัวอย่าง Code Python

```python
def decode_varint(buffer, offset):
    """
    ถอดรหัส varint จาก bytes
    return: (decoded_value, new_offset)
    """
    result = 0
    shift = 0
    
    while True:
        byte = buffer[offset]
        result |= ((byte & 0x7F) << shift)
        offset += 1
        if not (byte & 0x80):
            break
        shift += 7
    
    return result, offset

def decode_protobuf(buffer):
    """
    ถอดรหัส Protobuf message
    """
    offset = 0
    result = {}
    
    while offset < len(buffer):
        # อ่าน tag (field number และ wire type)
        tag, offset = decode_varint(buffer, offset)
        field_number = tag >> 3
        wire_type = tag & 0x07
        
        # ถอดรหัสตาม wire type
        if wire_type == 0:  # Varint
            value, offset = decode_varint(buffer, offset)
            result[f"field_{field_number}"] = value
            
        elif wire_type == 2:  # Length-delimited (string)
            length, offset = decode_varint(buffer, offset)
            value = buffer[offset:offset+length].decode('utf-8')
            result[f"field_{field_number}"] = value
            offset += length
            
    return result

# ตัวอย่างการใช้งาน
hex_data = bytes.fromhex("089601120774657374696E671A0568656C6C6F")
decoded = decode_protobuf(hex_data)
print(decoded)  # {'field_1': 150, 'field_2': 'testing', 'field_3': 'hello'}
```