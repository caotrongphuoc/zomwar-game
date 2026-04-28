# HƯỚNG DẪN BẮT ĐẦU LẬP TRÌNH GAME VỚI AK-BASE-KIT

Chào mừng các bạn đến với dự án phát triển Game trên vi điều khiển STM32L151! Repository này cung cấp bộ source code nền tảng (Base Kit) cùng tài liệu hướng dẫn chi tiết, giúp bạn nhanh chóng làm quen với kiến trúc hệ thống và bắt tay vào lập trình tựa game của riêng mình.

## I. Tạo :Sân chơi riêng" (Fork)

Để bắt đầu dự án cá nhân của mình, các bạn thực hiện đúng các bước sau:

- Đầu tiên các bạn truy cấp vào đường link sau: https://github.com/the-ak-foundation/ak-base-kit-stm32l151

- Fork: Nhấn nút Fork ở góc trên bên phải của repo này để copy dự án về tài khoản cá nhân của bạn.

<p align="center"><img width="1141" height="258" alt="Screenshot from 2026-04-28 14-20-55" src="https://github.com/user-attachments/assets/591ff540-c556-4fdc-9fec-e1974bd7cdd2" /></p>

- Create: Tạo bản fork

<p align="center"><img width="672" height="518" alt="image" src="https://github.com/user-attachments/assets/4541eb61-2a45-451e-8670-7e2e340f7e03" /></p>

- Sau khi create sẽ xuât hiện repository ak-base-kit-stm32l151, đây là "bộ khung" cơ bản để các bạn có thể phát triển con game của riêng mình:

<p align="center"><img width="1908" height="826" alt="Screenshot from 2026-04-28 14-47-53" src="https://github.com/user-attachments/assets/6d80c126-d6bc-4f36-9247-7320b3f1de9c" /></p>

## II. Các bước setup để build và chạy thử trên kit

Các bước setup các bạn làm theo link này nhé: https://epcb.vn/blogs/ak-embedded-software/ak-embedded-base-kit-stm32l151-getting-started

## III. Quy trình lập trình game hằng ngày

Vì chúng ta đang phát triển phần mềm nhúng, các bạn hãy sử dụng Terminal trên môi trường Ubuntu/Linux để thao tác cho chuẩn xác nhé.

### Bước 1: Kép repo cá nhân về máy (Chỉ làm 1 lần đầu)

Mở Terminal và chạy lệnh clone (nhớ thay bằng link repo của chính bạn): 
  
  git clone https://github.com/ten-cua-ban/ak-base-kit-stm32l151.git

### Bước 2: Modify Game

Toàn bộ logic game của bạn sẽ được viết bên trong thư mục application/.

### Bước 3: Lưu lại thay đổi và đẩy lên GitHub cá nhân

Sau khi code xong một tính năng (ví dụ: nhân vật bắn được đạn), hãy lưu lại tiến độ lên repo của bạn:
  
  git add .
  
  git commit -m "Tính năng: Thêm logic bắn đạn cho Peashooter"
  
  git push origin main
Sau bước này, mọi người có thể trực tiếp vào đường link GitHub của bạn để xem tiến độ và trải nghiệm code game bạn viếtviết




