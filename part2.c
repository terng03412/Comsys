static Node Expr()
{
    register Node root, result;
    // recurse ไปที่ Term() ตามกฏของ grammar แล้วเมื่อได้ผลลัพธ์ จาก Term() ให้ assign ค่าผลลัพธ์ไปที่ root
    // ที่จะเป็นตัวแทนของ subtree ทางด้านซ้าย (left subtree) ถ้าพบว่ามี sym ถัดไปเป็น + หรือ -
    root = Term();
    while ((sym == plus) || (sym == minus))
    {
        result = malloc(sizeof(NodeDesc)); // สร้าง Node ใหม่สำหรับ operator sym ที่ "กิน" เข้ามา
        result->kind = sym;                // กำหนดค่าให้ field kind ใน NodeDesc มีค่าเป็น plus หรือ minus
        sym = SGet();                      // "กิน" sym ตัวใหม่เข้ามาก
        result->left = root;               // นำ root ที่ถูก assign ก่อนจะเข้าใน while loop มาเป็น left subtree ของ result
        result->right = Term();            // recurse ไปที่ Term() แล้วนำผลลัพธ์ที่ได้มาเป็น right subtree ของ result
        root = result;                     // เมื่อจบการวนลูป while หนึ่งรอบ reassign ค่า root ให้เท่ากับ result
    }
    return root;
}