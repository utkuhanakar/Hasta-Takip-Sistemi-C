# 🏥 Gelişmiş Hasta Takip ve Analiz Sistemi v2.0

**Samsun Üniversitesi Yazılım Mühendisliği 1. Sınıf "Programlamaya Giriş" Dersi Projesi**

Bu proje, C programlama dilinde geliştirilmiş, **Dinamik Bellek Yönetimi (Dynamic Memory)** ve **Dosya İşlemleri (File I/O)** temellerine dayanan kapsamlı bir konsol uygulamasıdır. Başlangıçta verileri sadece RAM üzerinde tutan sistem (v1.0), Ödev 4 kapsamında geliştirilerek verileri kaydedebilen, okuyabilen ve loglayan kalıcı bir yapıya (v2.0) dönüştürülmüştür.

## 🚀 v2.0 Güncellemesi (Yeni Özellikler)

Proje, "Ödev 3" gereksinimlerinin üzerine "Ödev 4" isterlerini ekleyerek şu yetenekleri kazanmıştır:

* **💾 Veri Kalıcılığı (Data Persistence):** Program kapansa bile veriler kaybolmaz.
* **📂 CSV Desteği (Geleneksel):** Verileri virgülle ayrılmış dosyalar (`.csv`) olarak kaydeder ve geri yükler. `fprintf` ve `fscanf` kullanılarak geleneksel yöntemle kodlanmıştır.
* **🌐 JSON Desteği (Vibe Coding):** Verileri modern web standardı olan JSON formatında (`.json`) işler. Harici kütüphane kullanılmadan, **kendi yazdığım parser (ayrıştırıcı)** algoritması ile string işleme (string manipulation) yapılarak kodlanmıştır.
* **📝 Sistem Loglama:** Programın açılışı, kapanışı, veri ekleme/silme ve dosya işlemleri tarih ve saat damgasıyla `log.txt` dosyasına otomatik kaydedilir.

## 🛠️ Teknik Özellikler

Bu proje aşağıdaki bilgisayar bilimleri kavramlarını içerir:

* **Dinamik Bellek Yönetimi:** `malloc`, `realloc` ve `free` ile Heap yönetimi.
* **Struct & Pointer:** Veri yapılarını nesne yönelimli benzeri bir yapıda tutma.
* **File I/O:** `fopen`, `fclose`, dosya modları (`w`, `r`, `a`) kullanımı.
* **Algoritmalar:**
    * **Bubble Sort:** Hastaları sağlık puanına göre sıralama.
    * **Linear Search:** İsim ve aralık arama.
    * **Custom Parsing:** JSON verisini okumak için özel string ayrıştırma.
* **UI/UX:** ASCII tabanlı Canlı Dashboard ve Triage (Acil Durum) ekranı.

## 📸 Ekran Görüntüleri

### Canlı Dashboard ve Ana Menü
<img width="422" height="561" alt="Ekran görüntüsü 2025-12-22 224712" src="https://github.com/user-attachments/assets/58ccfba8-6d7f-49fe-97ca-d24e322e5ffb" />


### Acil Durum (Triage) Filtresi
<img width="446" height="427" alt="image" src="https://github.com/user-attachments/assets/c9332859-bdfe-4f5d-a58a-c7703aa6bde9" />


### JSON ve CSV Çıktıları
Proje çalıştırıldığında oluşturulan örnek veri dosyaları:
- `veri.csv`: Excel uyumlu veri saklama.
- `veri.json`: Web uyumlu veri saklama.
- `log.txt`: Sistem hareket dökümü.

## 📥 Kurulum ve Çalıştırma

1.  Projeyi klonlayın veya zip olarak indirin.
2.  `main.c` dosyasını bir C derleyicisinde (Dev-C++, GCC, VS Code) açın.
3.  Derleyin ve çalıştırın.
4.  Menüden **9** veya **11**'i seçerek ilk dosya kaydınızı oluşturun.

## 🙏 Teşekkür

Bu projede vizyonlarıyla yol gösteren ve **Vibe Coding** (Yapay Zeka ile Hibrit Kodlama) metodolojisini öğreten değerli hocalarım:
* **Dr. Öğr. Üyesi Nurettin Şenyer**
* **Öğr. Gör. İlker Gür**'e teşekkür ederim.

---
*Geliştirici: Utkuhan AKAR*
*Tarih: 22 Aralık 2025*
