# Linux-Syslog-LinkedList
# Linux Syslog Yönetimi - Çift Yönlü Bağlı Liste Projesi

Bu proje, bir yazılım öğrencisi olarak Linux işletim sistemindeki **Syslog (Sistem Günlükleri)** mekanizmasını anlamak ve bu verileri **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısı ile yönetmek amacıyla geliştirilmiştir.

## 📝 Ödev Bilgileri
* **Ad Soyad:** Sümeyye Sel
* **Öğrenci No:** 1240505014
* **Konu:** Syslog bilgilerinin bağlı liste ile modellenmesi

## 🚀 Teknik Detaylar & Soruların Cevapları

### 1. Syslog Mekanizması Nedir?
Syslog, sistemde meydana gelen her türlü olayı (hata, uyarı, bilgi mesajı) merkezi bir şekilde kaydeden bir protokoldür. Sistemin "kara kutusu" gibidir; hata ayıklama ve güvenlik izleme için kullanılır.

### 2. Neden Çift Yönlü Bağlı Liste?
Bu projede **Çift Yönlü Bağlı Liste** tercih edilmiştir çünkü:
* **Dinamik Bellek:** Logların sayısı önceden bilinmediği için diziler yerine çalışma zamanında büyüyebilen bir yapı gerekmiştir.
* **Hızlı Erişim ($O(1)$):** Kuyruk (`tail`) işaretçisi sayesinde yeni loglar listenin sonuna anında eklenir.
* **Geriye Dönük Analiz:** Sistem yöneticileri genellikle en son (güncel) logları görmek ister. Çift yönlü yapı sayesinde listede sondan başa doğru (yeniden eskiye) tarama yapmak çok hızlıdır.

## 🛠️ Fonksiyonlar ve Amacı
* `syslog_ekle()`: Bellekte dinamik yer ayırarak (malloc) yeni bir günlüğü listenin sonuna ekler.
* `listele_eskiden_yeniye()`: Logları kronolojik sırada listeler.
* `listele_yeniden_eskiye()`: Logları tersten tarayarak en güncel bilgileri gösterir.
* `bellek_bosalt()`: Program sonunda belleği temizleyerek sızıntıları (leak) önler.

## 💻 Çalıştırma Talimatı
1. Kod dosyasını indirin: `main.c`
2. Bir C derleyicisi (Dev-C++, GCC vb.) ile derleyin.
3. Uygulamayı çalıştırarak hem ileri hem geri yönlü log analizini terminalde görün.
