# HƯỚNG DẪN BẮT ĐẦU LẬP TRÌNH GAME VỚI AK-BASE-KIT

Chào mừng các bạn đến với dự án phát triển Game trên vi điều khiển STM32L151! Repository này cung cấp bộ source code nền tảng (Base Kit) cùng tài liệu hướng dẫn chi tiết, giúp bạn nhanh chóng làm quen với kiến trúc hệ thống và bắt tay vào lập trình tựa game của riêng mình.

## I. Tạo :Sân chơi riêng" (Fork)

Để bắt đầu dự án cá nhân của mình, các bạn thực hiện đúng các bước sau:

- Đầu tiên các bạn truy cấp vào đường link sau: https://github.com/the-ak-foundation/ak-base-kit-stm32l151

- Fork: Nhấn nút Fork ở góc trên bên phải của repo này để copy dự án về tài khoản cá nhân của bạn.

<p align="center"><img width="1141" height="258" alt="Screenshot from 2026-04-28 14-20-55" src="https://github.com/user-attachments/assets/591ff540-c556-4fdc-9fec-e1974bd7cdd2" /></p>

- Create: Tạo bản fork

<p align="center"><img width="618" height="461" alt="image" src="https://github.com/user-attachments/assets/378721d5-26c5-4898-b523-7e07594f6583" /></p>

*Note: 

- Tên của repo cũng sẽ là tên con game của bạn.

- Các bạn có thể mô tả về con game của bạn trong phần description.

Sau khi create, GitHub hiển thị repo của ak-base-kit-stm32l151, đây là "bộ khung" cơ bản để bạn có thể phát triển và hoàn thiện game của mình:

<p align="center"><img width="1908" height="826" alt="Screenshot from 2026-04-28 14-47-53" src="https://github.com/user-attachments/assets/6d80c126-d6bc-4f36-9247-7320b3f1de9c" /></p>

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

Toàn bộ logic game của bạn sẽ được viết bên trong thư mục application/.

### Bước 3: Lưu lại thay đổi và đẩy lên GitHub cá nhân

Sau khi code xong một tính năng (ví dụ: nhân vật bắn được đạn), hãy lưu lại tiến độ lên repo của bạn:
  
    git add .
  
    git commit -m "Tính năng: Thêm logic bắn đạn cho Peashooter"
  
    git push origin main
  
Sau bước này, mọi người có thể trực tiếp vào đường link GitHub của bạn để xem tiến độ và trải nghiệm code game bạn viếtviết




