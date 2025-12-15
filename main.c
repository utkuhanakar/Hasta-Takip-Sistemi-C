#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

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

void init_store(PatientStore *depo);
void add_patient(PatientStore *depo, int id, char *isim, int puan);
void list_patients(PatientStore *depo);
void update_scores_by_id(PatientStore *depo, int id, int yeni_puan);
void delete_by_id(PatientStore *depo, int id);
void search_by_name(PatientStore *depo, char *aranan);
void sort_by_average(PatientStore *depo);

void show_dashboard(PatientStore *depo);
void filter_emergency(PatientStore *depo);
void search_by_range(PatientStore *depo, int min, int max);

int main() {
    setlocale(LC_ALL, "Turkish");
    
    PatientStore sistem;
    init_store(&sistem);

    int secim = -1;
    int gecici_id;
    int gecici_puan;
    int min_puan, max_puan;
    char gecici_isim[50];

    while (secim != 0) {
        show_dashboard(&sistem);

        printf("\n");
        printf("+--------------------------------------+\n");
        printf("|          ANA MENU SECENEKLERI        |\n");
        printf("+--------------------------------------+\n");
        printf("| 1. Hasta Ekle    (add_patient)       |\n");
        printf("| 2. Listele       (list_patients)     |\n");
        printf("| 3. Puan Guncelle (update_scores)     |\n");
        printf("| 4. Sil           (delete_by_id)      |\n");
        printf("| 5. Isimle Ara    (Vibe Coding)       |\n");
        printf("| 6. Sirala        (Vibe Coding)       |\n");
        printf("| 7. Aralik Arama  (Range Search)      |\n");
        printf("| 8. ACIL DURUM    (Triage Filter)     |\n");
        printf("| 0. Cikis                             |\n");
        printf("+--------------------------------------+\n");
        printf("Seciminiz: ");
        scanf("%d", &secim);

        if (secim == 1) {
            printf("\n--- YENI KAYIT ---\n");
            printf("ID: ");
            scanf("%d", &gecici_id);
            printf("Ad Soyad: ");
            scanf(" %[^\n]s", gecici_isim);
            
            do {
                printf("Saglik Puani (0-100): ");
                scanf("%d", &gecici_puan);
                if(gecici_puan < 0 || gecici_puan > 100) {
                    printf("Hata: Puan 0-100 arasinda olmali.\n");
                }
            } while(gecici_puan < 0 || gecici_puan > 100);

            add_patient(&sistem, gecici_id, gecici_isim, gecici_puan);
        }
        else if (secim == 2) {
            list_patients(&sistem);
        }
        else if (secim == 3) {
            printf("\n--- GUNCELLEME ---\n");
            printf("ID Giriniz: ");
            scanf("%d", &gecici_id);
            do {
                printf("Yeni Puan: ");
                scanf("%d", &gecici_puan);
            } while(gecici_puan < 0 || gecici_puan > 100);
            update_scores_by_id(&sistem, gecici_id, gecici_puan);
        }
        else if (secim == 4) {
            printf("\n--- SILME ---\n");
            printf("Silinecek ID: ");
            scanf("%d", &gecici_id);
            delete_by_id(&sistem, gecici_id);
        }
        else if (secim == 5) {
            printf("\n--- ARAMA ---\n");
            printf("Aranacak metin: ");
            scanf(" %[^\n]s", gecici_isim);
            search_by_name(&sistem, gecici_isim);
        }
        else if (secim == 6) {
            sort_by_average(&sistem);
            printf("\nListe siralandi. Listeleme yapabilirsiniz.\n");
        }
        else if (secim == 7) {
            printf("\n--- ARALIK ARAMA ---\n");
            printf("Minimum Puan: ");
            scanf("%d", &min_puan);
            printf("Maksimum Puan: ");
            scanf("%d", &max_puan);
            search_by_range(&sistem, min_puan, max_puan);
        }
        else if (secim == 8) {
            filter_emergency(&sistem);
        }
        else if (secim == 0) {
            printf("Cikis yapiliyor...\n");
            if (sistem.data != NULL) {
                free(sistem.data);
            }
        }
        else {
            printf("Gecersiz secim.\n");
        }
    }

    return 0;
}

void init_store(PatientStore *depo) {
    depo->size = 0;
    depo->cap = 2;
    depo->baslangic_zamani = time(NULL);
    depo->data = (Patient *)malloc(depo->cap * sizeof(Patient));
    if (depo->data == NULL) {
        printf("Hata: Bellek yetersiz.\n");
        exit(1);
    }
}

void show_dashboard(PatientStore *depo) {
    int i;
    int kritik = 0;
    time_t su_an = time(NULL);
    double calisma_suresi = difftime(su_an, depo->baslangic_zamani);
    
    for(i = 0; i < depo->size; i++) {
        if(depo->data[i].health_score < 20) {
            kritik++;
        }
    }

    printf("\n==========================================\n");
    printf("   HASTA TAKIP VE ANALIZ PANELI\n");
    printf("==========================================\n");
    printf("   Calisma Suresi : %.0f saniye\n", calisma_suresi);
    printf("   Toplam Hasta   : %d\n", depo->size);
    printf("   Kapasite       : %d\n", depo->cap);
    printf("   ACIL DURUM (!) : %d kisi\n", kritik);
    printf("==========================================\n");
}

void add_patient(PatientStore *depo, int id, char *isim, int puan) {
    int i;
    for(i = 0; i < depo->size; i++) {
        if(depo->data[i].id == id) {
            printf("Hata: Bu ID (%d) zaten kayitli!\n", id);
            return;
        }
    }

    if (depo->size >= depo->cap) {
        int yeni_boyut = depo->cap * 2;
        Patient *yeni_yer = (Patient *)realloc(depo->data, yeni_boyut * sizeof(Patient));
        if (yeni_yer == NULL) {
            printf("Hata: RAM yetersiz.\n");
            return;
        }
        depo->data = yeni_yer;
        depo->cap = yeni_boyut;
    }

    depo->data[depo->size].id = id;
    strcpy(depo->data[depo->size].name, isim);
    depo->data[depo->size].health_score = puan;
    depo->size++;
    printf("Hasta eklendi: %s\n", isim);
}

void list_patients(PatientStore *depo) {
    if (depo->size == 0) {
        printf("Liste bos.\n");
        return;
    }

    printf("\n+-------+--------------------------+------+\n");
    printf("|  ID   | ISIM SOYISIM             | PUAN |\n");
    printf("+-------+--------------------------+------+\n");
    
    int i;
    for (i = 0; i < depo->size; i++) {
        printf("| %-5d | %-24s | %-4d |\n", 
               depo->data[i].id, 
               depo->data[i].name, 
               depo->data[i].health_score);
    }
    printf("+-------+--------------------------+------+\n");
}

void update_scores_by_id(PatientStore *depo, int id, int yeni_puan) {
    int i;
    for (i = 0; i < depo->size; i++) {
        if (depo->data[i].id == id) {
            depo->data[i].health_score = yeni_puan;
            printf("Puan guncellendi.\n");
            return;
        }
    }
    printf("Kayit bulunamadi.\n");
}

void delete_by_id(PatientStore *depo, int id) {
    int i, j;
    int bulundu = 0;
    for (i = 0; i < depo->size; i++) {
        if (depo->data[i].id == id) {
            bulundu = 1;
            for (j = i; j < depo->size - 1; j++) {
                depo->data[j] = depo->data[j + 1];
            }
            depo->size--;
            printf("Kayit silindi.\n");
            break; 
        }
    }
    if (bulundu == 0) {
        printf("Silinecek ID bulunamadi.\n");
    }
}

void search_by_name(PatientStore *depo, char *aranan) {
    int i;
    int sayac = 0;
    
    printf("\n+-------+--------------------------+------+\n");
    printf("|  ID   | ISIM SOYISIM             | PUAN |\n");
    printf("+-------+--------------------------+------+\n");

    for (i = 0; i < depo->size; i++) {
        if (strstr(depo->data[i].name, aranan) != NULL) {
            printf("| %-5d | %-24s | %-4d |\n", 
                   depo->data[i].id, 
                   depo->data[i].name, 
                   depo->data[i].health_score);
            sayac++;
        }
    }
    printf("+-------+--------------------------+------+\n");
    
    if (sayac == 0) {
        printf("Sonuc bulunamadi.\n");
    }
}

void sort_by_average(PatientStore *depo) {
    int i, j;
    Patient gecici;
    
    for (i = 0; i < depo->size - 1; i++) {
        for (j = 0; j < depo->size - i - 1; j++) {
            if (depo->data[j].health_score < depo->data[j + 1].health_score) {
                gecici = depo->data[j];
                depo->data[j] = depo->data[j + 1];
                depo->data[j + 1] = gecici;
            }
        }
    }
}

void search_by_range(PatientStore *depo, int min, int max) {
    int i;
    int sayac = 0;

    printf("\n--- %d ile %d Puan Arasi Sonuc ---\n", min, max);
    printf("+-------+--------------------------+------+\n");
    printf("|  ID   | ISIM SOYISIM             | PUAN |\n");
    printf("+-------+--------------------------+------+\n");

    for (i = 0; i < depo->size; i++) {
        if (depo->data[i].health_score >= min && depo->data[i].health_score <= max) {
            printf("| %-5d | %-24s | %-4d |\n", 
                   depo->data[i].id, 
                   depo->data[i].name, 
                   depo->data[i].health_score);
            sayac++;
        }
    }
    printf("+-------+--------------------------+------+\n");
    if (sayac == 0) printf("Kayit yok.\n");
}

void filter_emergency(PatientStore *depo) {
    int i;
    int sayac = 0;

    printf("\n!!! ACIL DURUM LISTESI (Puan < 20) !!!\n");
    printf("+-------+--------------------------+------+\n");
    printf("|  ID   | ISIM SOYISIM             | PUAN |\n");
    printf("+-------+--------------------------+------+\n");

    for (i = 0; i < depo->size; i++) {
        if (depo->data[i].health_score < 20) {
            printf("| %-5d | %-24s | %-4d |\n", 
                   depo->data[i].id, 
                   depo->data[i].name, 
                   depo->data[i].health_score);
            sayac++;
        }
    }
    printf("+-------+--------------------------+------+\n");
    if (sayac == 0) printf("Acil hasta yok.\n");
}
