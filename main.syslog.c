/*
 * Ad Soyad: Sümeyye Sel
 * Öðrenci No: 1240505014
 * Proje: Linux Syslog Yönetimi (Çift Yönlü Baðlý Liste Uygulamasý)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @struct LogDugumu
 * @brief Sistem günlüklerini (syslog) temsil eden düðüm yapýsý.
 * Dinamik bellek yönetimi için iþaretçiler (pointers) kullanýlmýþtýr.
 */
typedef struct SistemGunlukDugumu {
    char zaman_damgasi[32];    // Olayýn gerçekleþme aný
    char hata_seviyesi[12];    // INFO, WARNING, ERROR gibi kategoriler
    char mesaj_icerigi[256];   // Teknik detay mesajý
    struct SistemGunlukDugumu *sonraki_kayit; // Bir sonraki log düðümünün adresi
    struct SistemGunlukDugumu *onceki_kayit;  // Bir önceki log düðümünün adresi
} LogDugumu;

// Liste kontrolünü saðlayan küresel (global) iþaretçiler
LogDugumu *liste_basi = NULL; // Listenin baþlangýç noktasýný tutar
LogDugumu *liste_sonu = NULL; // Listenin bitiþ noktasýný tutar

/**
 * @brief Sistemden güncel saat bilgisini alarak formatlar.
 */
void su_anki_zamani_al(char *zaman_kutusu) {
    time_t ham_zaman;
    struct tm *zaman_bilgisi;
    time(&ham_zaman);
    zaman_bilgisi = localtime(&ham_zaman);
    strftime(zaman_kutusu, 32, "%b %d %H:%M:%S", zaman_bilgisi);
}

/**
 * @brief Yeni bir sistem günlüðü (log) oluþturur ve listenin sonuna ekler.
 * Dinamik bellek kullanýmý sayesinde dizi sýnýrý olmadan log tutulabilir.
 */
void syslog_ekle(const char *seviye, const char *mesaj) {
    // malloc() ile Heap bellekte yeni bir düðüm alaný açýyoruz.
    LogDugumu *yeni_dugum = (LogDugumu *)malloc(sizeof(LogDugumu));
    
    if (yeni_dugum == NULL) {
        printf("Hata: Bellek tahsis edilemedi!\n");
        return;
    }

    // Bilgileri düðüm içerisine kopyalýyoruz
    su_anki_zamani_al(yeni_dugum->zaman_damgasi);
    strncpy(yeni_dugum->hata_seviyesi, seviye, 12);
    strncpy(yeni_dugum->mesaj_icerigi, mesaj, 256);
    yeni_dugum->sonraki_kayit = NULL;

    // Baðlý Liste Ýþlemleri:
    if (liste_basi == NULL) {
        // Liste boþsa yeni düðüm listenin tek elemanýdýr.
        yeni_dugum->onceki_kayit = NULL;
        liste_basi = yeni_dugum;
        liste_sonu = yeni_dugum;
    } else {
        // Liste doluysa yeni düðümü sonuncunun sonuna ekle.
        yeni_dugum->onceki_kayit = liste_sonu;
        liste_sonu->sonraki_kayit = yeni_dugum;
        liste_sonu = yeni_dugum; // Güncel sonuncu artýk bu düðümdür.
    }
}

/**
 * @brief Loglarý eskiden yeniye (doðru kronolojik sýra) yazdýrýr.
 */
void listele_eskiden_yeniye() {
    printf("\n--- SYSLOG: ESKIDEN YENIYE (BASTAN SONA) ---\n");
    LogDugumu *gecici = liste_basi;
    while (gecici != NULL) {
        printf("[%s] %s: %s\n", gecici->zaman_damgasi, gecici->hata_seviyesi, gecici->mesaj_icerigi);
        gecici = gecici->sonraki_kayit;
    }
}

/**
 * @brief Loglarý yeniden eskiye (ters kronolojik) yazdýrýr.
 * Çift yönlü baðlý liste sayesinde listenin sonundan baþýna kolayca gidilebilir.
 */
void listele_yeniden_eskiye() {
    printf("\n--- SYSLOG: YENIDEN ESKIYE (GUNCEL ANALIZ) ---\n");
    LogDugumu *gecici = liste_sonu;
    while (gecici != NULL) {
        printf("[%s] %s: %s\n", gecici->zaman_damgasi, gecici->hata_seviyesi, gecici->mesaj_icerigi);
        gecici = gecici->onceki_kayit;
    }
}

/**
 * @brief Bellek sýzýntýlarýný önlemek için kullanýlan düðümleri serbest býrakýr.
 */
void bellek_bosalt() {
    LogDugumu *gecici = liste_basi;
    while (gecici != NULL) {
        LogDugumu *silinecek = gecici;
        gecici = gecici->sonraki_kayit;
        free(silinecek);
    }
    liste_basi = liste_sonu = NULL;
}

int main() {
    // 1. Örnek Verilerin Eklenmesi
    syslog_ekle("BILGI", "Sistem servisleri baslatildi.");
    syslog_ekle("UYARI", "Bellek kullanimi %90'a ulasti.");
    syslog_ekle("HATA", "Baglanti zaman asimina ugradi.");
    syslog_ekle("BILGI", "Guncellemeler denetleniyor.");

    // 2. Çýktýlarýn Gösterilmesi
    listele_eskiden_yeniye();
    listele_yeniden_eskiye();

    // 3. Güvenli Kapanýþ (Bellek Temizliði)
    bellek_bosalt();

    printf("\nProgram basariyla calisti. Cikmak icin bir tusa basin...");
    getchar();
    return 0;
}
