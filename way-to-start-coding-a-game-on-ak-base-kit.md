# HƯỚNG DẪN BẮT ĐẦU LẬP TRÌNH GAME VỚI AK-BASE-KIT

Chào mừng các bạn đến với dự án phát triển Game trên vi điều khiển STM32L151! Repository này cung cấp bộ source code nền tảng (Base Kit) cùng tài liệu hướng dẫn chi tiết, giúp bạn nhanh chóng làm quen với kiến trúc hệ thống và bắt tay vào lập trình tựa game của riêng mình.

## I. Tạo "Sân chơi riêng" (Fork)

Để bắt đầu dự án cá nhân của mình, các bạn thực hiện đúng các bước sau:

- Đầu tiên các bạn truy cập vào đường link sau: https://github.com/the-ak-foundation/ak-base-kit-stm32l151

- Fork: Nhấn nút Fork ở góc trên bên phải của repo này để copy dự án về tài khoản cá nhân của bạn.

<p align="center"><img width="1141" height="258" alt="Screenshot from 2026-04-28 14-20-55" src="https://github.com/user-attachments/assets/591ff540-c556-4fdc-9fec-e1974bd7cdd2" /></p>

- Create: Tạo bản fork

<p align="center"><img width="618" height="461" alt="image" src="https://github.com/user-attachments/assets/378721d5-26c5-4898-b523-7e07594f6583" /></p>

*Note: 

- Tên của repo cũng sẽ là tên con game của bạn.

- Các bạn có thể mô tả về con game của bạn trong phần description.

Sau khi create, GitHub hiển thị repo của ak-base-kit-stm32l151, đây là "bộ khung" cơ bản để bạn có thể phát triển và hoàn thiện game của mình:

<p align="center"><img width="2523" height="1000" alt="image" src="https://github.com/user-attachments/assets/37821fde-90e9-4c4e-babb-2c7dc03fa07c" /></p>

## II. Các bước setup để build và chạy thử trên kit

Các bước setup các bạn làm theo link này nhé: https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-getting-started

## III. Quy trình lập trình game hằng ngày

Vì chúng ta đang phát triển phần mềm nhúng, các bạn hãy sử dụng Terminal trên môi trường Ubuntu/Linux để thao tác cho chuẩn xác nhé.

### Bước 0: Tạo thư mục làm việc

Từ Home, các bạn tạo một thư mục làm việc (Workspace) - Đây là nơi chứa các source code cũng như các công cụ lập trình.

<p align="center"><img width="966" height="314" alt="image" src="https://github.com/user-attachments/assets/60eff60e-e2d2-4f8e-9183-c67934ea0a28" /></p>

Trong Workspace, các bạn tạo 2 thư mục:

- Sources: nơi chứa các dự án lập trình của bạn.
- Tools: nơi chứa các công cụ lập trình của bạn (Các công cụ lập trình các bạn có thể tham khảo trong phần II).

<p align="center"><img width="1376" height="468" alt="image" src="https://github.com/user-attachments/assets/b1fa5a0f-ef28-4eba-b823-bec7030117fb" /></p>

### Bước 1: Kép repo cá nhân về máy (Chỉ làm 1 lần đầu)

Trong thư mục Source đã tạo ở bước 0, các bạn mở Terminal và chạy lệnh clone (nhớ thay bằng link repo của chính bạn): 
  
    git clone https://github.com/ten-cua-ban/ten-repo-da-fork-cua-ban.git

  <p align="center"><img width="1179" height="258" alt="image" src="https://github.com/user-attachments/assets/6ba6e15d-ce85-483e-9519-61fcb017dafb" /></p>

### Bước 2: Modify Game

Các bạn mở VSCode trên Linux rồi mở repo mình mới fork rồi bắt đầu lập trình game nhé.

Toàn bộ logic game của bạn sẽ được viết bên trong thư mục application/.

<p align="center"><img width="1474" height="910" alt="image" src="https://github.com/user-attachments/assets/c403a5a8-80c3-42fc-a96f-499c05c92a73" /></p>

#### Ví dụ cập nhật logo của AK Foundation

- Tạo file header trong thư mục screens:

<p align="center"><img width="920" height="502" alt="image" src="https://github.com/user-attachments/assets/aa019658-4519-4cdd-a12e-900bf925133a" /></p>

- Tạo file xử lý bitmap trong thư mục screens:

<p align="center"><img width="1265" height="884" alt="image" src="https://github.com/user-attachments/assets/04768ea3-f7d9-44ba-b0cd-b00853e706cc" /></p>

- Include file header vào scr_startup_h:

<p align="center"><img width="871" height="393" alt="image" src="https://github.com/user-attachments/assets/f15b90a6-9d31-4b5b-aaee-93a6644003b7" /></p>

- Trong file scr_startup.cpp, cập nhật lại hàm view_scr_startup():

<p align="center"><img width="876" height="381" alt="image" src="https://github.com/user-attachments/assets/9d497da1-3adf-4f9c-83bf-7bbdbe6580c0" /></p>

- Build và nạp lên kit (Casch build và nạp các bạn có thể tham khảo phần II nhé):

<p align="center"><img width="828" height="1430" alt="a23f6fefedea6cb435fb" src="https://github.com/user-attachments/assets/d9032475-3f4c-4f79-8073-ecb10345333f" /></p>

### Bước 3: Lưu lại thay đổi và đẩy lên GitHub cá nhân

Sau khi code xong một tính năng (như ví dụ trên), hãy lưu lại tiến độ lên repo của bạn bằng cách gõ các lệnh này trên termianal (Chú ý đường dẫn phải đang là ngay thư mục gốc):
  
    git add .
  
    git commit -m "Tính năng: Cập nhật logo AK Foundation"
  
    git push origin main

Kết qủa sau ghi gõ 3 lệnh trên:
 
 <p align="center"><img width="860" height="257" alt="image" src="https://github.com/user-attachments/assets/ff8edf7a-e368-42c2-82dc-123d16843b01" /></p>

Repo của bạn đã được cập nhật trên GitHub:

 <p align="center"><img width="1188" height="359" alt="image" src="https://github.com/user-attachments/assets/d09a9419-0d1b-47d8-9148-ef494f58932a" /></p>

Sau bước này, mọi người có thể trực tiếp vào đường link GitHub của bạn để xem tiến độ và trải nghiệm code game bạn viết:






