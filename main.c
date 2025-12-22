#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

// --- YAPILAR (STRUCTS) ---
typedef struct {
    int id;
    char name[50];
    int health_score;
} Patient;

typedef struct {
    Patient *data;
    int size;
    int cap;
    time_t baslangic_zamani;
} PatientStore;

// --- FONKSIYON PROTOTIPLERI ---
void init_store(PatientStore *store);
void add_patient(PatientStore *store, int id, char *isim, int puan);
void list_patients(PatientStore *store);
void update_scores_by_id(PatientStore *store, int id, int yeni_puan);
void delete_by_id(PatientStore *store, int id);
void search_by_name(PatientStore *store, char *aranan);
void sort_by_average(PatientStore *store);

// Analiz Fonksiyonlari
void show_dashboard(PatientStore *store);
void filter_emergency(PatientStore *store);
void search_by_range(PatientStore *store, int min, int max);

// --- ODEV 4 FONKSIYONLARI (ZORUNLU) ---
// Geleneksel Yontemler
int save_patients_to_csv(const PatientStore *ps, const char *filename);
int load_patients_from_csv(PatientStore *ps, const char *filename);

// Vibe Coding Yontemleri
int save_patients_to_json(const PatientStore *ps, const char *filename);
int load_patients_from_json(PatientStore *ps, const char *filename);
void log_process(const char *message);

int main() {
    setlocale(LC_ALL, "Turkish");
    
    // Program baslangicini logla [cite: 16]
    log_process("Program Baslatildi");

    PatientStore sistem;
    init_store(&sistem);

    int secim = -1;
    int temp_id, temp_puan, min_p, max_p;
    char temp_isim[50];
    char dosya_adi[50];

    while (secim != 0) {
        show_dashboard(&sistem);

        printf("\n");
        printf("+------------------------------------------+\n");
        printf("|          ANA MENU SECENEKLERI            |\n");
        printf("+------------------------------------------+\n");
        printf("| 1. Hasta Ekle      (RAM)                 |\n");
        printf("| 2. Listele         (RAM)                 |\n");
        printf("| 3. Puan Guncelle   (RAM)                 |\n");
        printf("| 4. Sil             (RAM)                 |\n");
        printf("| 5. Isimle Ara      (Vibe Coding)         |\n");
        printf("| 6. Sirala          (Vibe Coding)         |\n");
        printf("| 7. Aralik Arama    (Range Search)        |\n");
        printf("| 8. ACIL DURUM      (Triage Filter)       |\n");
        printf("| ---------------------------------------- |\n");
        printf("| 9. CSV Kaydet      (Geleneksel) [ODEV 4] |\n");
        printf("| 10. CSV Yukle      (Geleneksel) [ODEV 4] |\n");
        printf("| 11. JSON Kaydet    (Vibe Coding)[ODEV 4] |\n");
        printf("| 12. JSON Yukle     (Vibe Coding)[ODEV 4] |\n");
        printf("| 0. Cikis                                 |\n");
        printf("+------------------------------------------+\n");
        printf("Seciminiz: ");
        scanf("%d", &secim);

        if (secim == 1) {
            printf("\n--- YENI KAYIT ---\n");
            printf("ID: "); scanf("%d", &temp_id);
            printf("Ad Soyad: "); scanf(" %[^\n]s", temp_isim); 
            do {
                printf("Saglik Puani (0-100): "); scanf("%d", &temp_puan);
            } while(temp_puan < 0 || temp_puan > 100);
            add_patient(&sistem, temp_id, temp_isim, temp_puan);
        }
        else if (secim == 2) list_patients(&sistem);
        else if (secim == 3) {
            printf("ID: "); scanf("%d", &temp_id);
            printf("Yeni Puan: "); scanf("%d", &temp_puan);
            update_scores_by_id(&sistem, temp_id, temp_puan);
        }
        else if (secim == 4) {
            printf("Silinecek ID: "); scanf("%d", &temp_id);
            delete_by_id(&sistem, temp_id);
        }
        else if (secim == 5) {
            printf("Aranacak metin: "); scanf(" %[^\n]s", temp_isim);
            search_by_name(&sistem, temp_isim);
        }
        else if (secim == 6) {
            sort_by_average(&sistem);
            printf("\nListe siralandi.\n");
        }
        else if (secim == 7) {
            printf("Min Puan: "); scanf("%d", &min_p);
            printf("Max Puan: "); scanf("%d", &max_p);
            search_by_range(&sistem, min_p, max_p);
        }
        else if (secim == 8) filter_emergency(&sistem);
        
        // --- ODEV 4 ISLEMLERI ---
        else if (secim == 9) { 
            printf("Dosya adi (orn: data.csv): "); scanf("%s", dosya_adi);
            if(save_patients_to_csv(&sistem, dosya_adi)) {
                log_process("Veriler CSV olarak kaydedildi"); // Loglama [cite: 16]
            }
        }
        else if (secim == 10) { 
            printf("Okunacak dosya (orn: data.csv): "); scanf("%s", dosya_adi);
            if(load_patients_from_csv(&sistem, dosya_adi)) {
                log_process("Veriler CSV dosyasindan yuklendi"); // Loglama [cite: 16]
            }
        }
        else if (secim == 11) { 
            printf("Dosya adi (orn: data.json): "); scanf("%s", dosya_adi);
            if(save_patients_to_json(&sistem, dosya_adi)) {
                log_process("Veriler JSON olarak kaydedildi"); // Loglama [cite: 16]
            }
        }
        else if (secim == 12) { 
            printf("Okunacak dosya (orn: data.json): "); scanf("%s", dosya_adi);
            if(load_patients_from_json(&sistem, dosya_adi)) {
                log_process("Veriler JSON dosyasindan yuklendi"); // Loglama [cite: 16]
            }
        }
        else if (secim == 0) {
            log_process("Program Kapatildi"); // Kapanis Logu [cite: 16]
            printf("Cikis yapiliyor... Bellek temizleniyor.\n");
            if (sistem.data != NULL) free(sistem.data);
        }
        else printf("Gecersiz secim.\n");
    }
    return 0;
}

// --- FONKSIYONLAR ---

void init_store(PatientStore *store) {
    store->size = 0;
    store->cap = 2;
    store->baslangic_zamani = time(NULL);
    store->data = (Patient *)malloc(store->cap * sizeof(Patient));
    if (store->data == NULL) exit(1);
}

// [cite: 6, 7] 1. CSV KAYDETME (Geleneksel Kodlama)
// Odevde istenen: int save_patients_to_csv(const PatientStore *ps, const char *filename)
int save_patients_to_csv(const PatientStore *ps, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Hata: Dosya olusturulamadi.\n");
        return 0;
    }
    
    // Basitce virgul ile ayirarak yaziyoruz (Geleneksel Yontem)
    int i;
    for (i = 0; i < ps->size; i++) {
        fprintf(fp, "%d,%s,%d\n", ps->data[i].id, ps->data[i].name, ps->data[i].health_score);
    }
    
    fclose(fp);
    printf(">> Basarili: CSV dosyasi olusturuldu.\n");
    return 1;
}

// [cite: 9, 10] 2. CSV YUKLEME (Geleneksel Kodlama)
// Odevde istenen: int load_patients_from_csv(PatientStore *ps, const char *filename)
int load_patients_from_csv(PatientStore *ps, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Hata: Dosya bulunamadi.\n");
        return 0;
    }
    
    int id, puan;
    char isim[50];
    
    // Dosyanin sonuna kadar fscanf ile oku
    // Format: %d,%[^,],%d -> Virgul gorene kadar string oku teknigi
    while (fscanf(fp, "%d,%[^,],%d\n", &id, isim, &puan) != EOF) {
        add_patient(ps, id, isim, puan);
    }
    
    fclose(fp);
    printf(">> Basarili: Veriler CSV'den eklendi.\n");
    return 1;
}

// [cite: 12, 13] 3. JSON KAYDETME (Vibe Coding)
// Yapay zeka destegi ile JSON formatina uygun manuel yazdirma
int save_patients_to_json(const PatientStore *ps, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) return 0;
    
    fprintf(fp, "[\n"); // JSON dizi baslangici
    int i;
    for (i = 0; i < ps->size; i++) {
        fprintf(fp, "  {\n");
        fprintf(fp, "    \"id\": %d,\n", ps->data[i].id);
        fprintf(fp, "    \"name\": \"%s\",\n", ps->data[i].name);
        fprintf(fp, "    \"health_score\": %d\n", ps->data[i].health_score);
        
        // Son elemanda virgul olmaz (JSON kurali)
        if (i < ps->size - 1) fprintf(fp, "  },\n");
        else fprintf(fp, "  }\n");
    }
    fprintf(fp, "]\n"); // JSON dizi bitisi
    
    fclose(fp);
    printf(">> Basarili: JSON dosyasi olusturuldu (Vibe Coding).\n");
    return 1;
}

// [cite: 12, 14] 4. JSON YUKLEME (Vibe Coding)
// Kutuphane kullanmadan string parsing ile "manuel" JSON okuma
int load_patients_from_json(PatientStore *ps, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Hata: Dosya bulunamadi.\n");
        return 0;
    }
    
    char satir[200];
    int temp_id = 0, temp_puan = 0;
    char temp_isim[50] = "";
    
    while (fgets(satir, sizeof(satir), fp)) {
        // "id": 10,  seklindeki satiri bul
        if (strstr(satir, "\"id\":")) {
            sscanf(satir, "    \"id\": %d", &temp_id);
        }
        // "name": "Ali", seklindeki satiri bul
        else if (strstr(satir, "\"name\":")) {
            char *ptr = strchr(satir, ':'); // :'dan sonrasina bak
            ptr = strchr(ptr, '"');         // Ilk tirnagi bul
            ptr++;                          // Ismin baslangici
            char *son = strchr(ptr, '"');   // Ikinci tirnagi bul
            if (son) *son = '\0';           // Stringi kapat
            strcpy(temp_isim, ptr);
        }
        // "health_score": 50 seklindeki satiri bul
        else if (strstr(satir, "\"health_score\":")) {
            sscanf(satir, "    \"health_score\": %d", &temp_puan);
            // Puan da okunduysa bu hasta bitmistir, ekleyelim
            add_patient(ps, temp_id, temp_isim, temp_puan);
        }
    }
    
    fclose(fp);
    printf(">> Basarili: JSON verileri islendi (Vibe Coding).\n");
    return 1;
}

// [cite: 15, 16] 5. LOGLAMA (Vibe Coding)
// Islem kayitlarini zamaniyla birlikte tutar
void log_process(const char *message) {
    FILE *fp = fopen("log.txt", "a"); // "a": Append (Ekleme) modu
    if (fp == NULL) return;
    
    time_t now;
    time(&now);
    char *zaman_str = ctime(&now);
    // ctime sonunda \n ile gelir, onu silelim ki satir duzgun olsun
    zaman_str[strlen(zaman_str) - 1] = '\0';
    
    fprintf(fp, "[%s] -> %s\n", zaman_str, message);
    fclose(fp);
}

// --- DIGER YARDIMCI FONKSIYONLAR ---
void show_dashboard(PatientStore *store) {
    int i, kritik = 0;
    for(i=0; i<store->size; i++) if(store->data[i].health_score < 20) kritik++;
    
    printf("\n==========================================\n");
    printf("   HASTA TAKIP VE ANALIZ PANELI v2.0\n");
    printf("==========================================\n");
    printf("   Toplam Hasta   : %d\n", store->size);
    printf("   Kapasite       : %d\n", store->cap);
    printf("   ACIL DURUM (!) : %d kisi\n", kritik);
    printf("==========================================\n");
}

void add_patient(PatientStore *store, int id, char *isim, int puan) {
    int i;
    for(i=0; i<store->size; i++) {
        if(store->data[i].id == id) {
            printf("Hata: Bu ID (%d) zaten var!\n", id);
            return;
        }
    }
    if (store->size >= store->cap) {
        store->cap *= 2;
        store->data = (Patient *)realloc(store->data, store->cap * sizeof(Patient));
    }
    store->data[store->size].id = id;
    strcpy(store->data[store->size].name, isim);
    store->data[store->size].health_score = puan;
    store->size++;
    printf("Hasta eklendi: %s\n", isim);
}

void list_patients(PatientStore *store) {
    if (store->size == 0) { printf("Liste Bos.\n"); return; }
    printf("\n+-------+--------------------------+------+\n");
    printf("|  ID   | ISIM SOYISIM             | PUAN |\n");
    printf("+-------+--------------------------+------+\n");
    int i;
    for (i = 0; i < store->size; i++) {
        printf("| %-5d | %-24s | %-4d |\n", store->data[i].id, store->data[i].name, store->data[i].health_score);
    }
    printf("+-------+--------------------------+------+\n");
}

void update_scores_by_id(PatientStore *store, int id, int yeni_puan) {
    int i;
    for (i = 0; i < store->size; i++) {
        if (store->data[i].id == id) {
            store->data[i].health_score = yeni_puan;
            printf("Guncellendi.\n"); return;
        }
    }
    printf("Bulunamadi.\n");
}

void delete_by_id(PatientStore *store, int id) {
    int i, j;
    for (i = 0; i < store->size; i++) {
        if (store->data[i].id == id) {
            for (j = i; j < store->size - 1; j++) {
                store->data[j] = store->data[j + 1];
            }
            store->size--;
            printf("Silindi.\n"); return;
        }
    }
    printf("Bulunamadi.\n");
}

void search_by_name(PatientStore *store, char *aranan) {
    int i, c=0;
    printf("\nSonuclar:\n");
    for (i = 0; i < store->size; i++) {
        if (strstr(store->data[i].name, aranan) != NULL) {
            printf("- %s (ID: %d, Puan: %d)\n", store->data[i].name, store->data[i].id, store->data[i].health_score);
            c++;
        }
    }
    if(c==0) printf("Bulunamadi.\n");
}

void sort_by_average(PatientStore *store) {
    int i, j;
    Patient temp;
    for (i = 0; i < store->size - 1; i++) {
        for (j = 0; j < store->size - i - 1; j++) {
            if (store->data[j].health_score < store->data[j + 1].health_score) {
                temp = store->data[j];
                store->data[j] = store->data[j + 1];
                store->data[j + 1] = temp;
            }
        }
    }
}

void search_by_range(PatientStore *store, int min, int max) {
    int i, c=0;
    printf("\n%d-%d Araligindaki Hastalar:\n", min, max);
    for (i = 0; i < store->size; i++) {
        if (store->data[i].health_score >= min && store->data[i].health_score <= max) {
            printf("- %s (Puan: %d)\n", store->data[i].name, store->data[i].health_score);
            c++;
        }
    }
    if(c==0) printf("Kayit yok.\n");
}

void filter_emergency(PatientStore *store) {
    int i, c=0;
    printf("\n!!! ACIL LISTESI !!!\n");
    for (i = 0; i < store->size; i++) {
        if (store->data[i].health_score < 20) {
            printf("! %s (ID: %d) -> Puan: %d\n", store->data[i].name, store->data[i].id, store->data[i].health_score);
            c++;
        }
    }
    if(c==0) printf("Acil hasta yok.\n");
}
