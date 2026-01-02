# Graph Community Detection

Bu proje, graf topluluk tespiti (community detection) algoritmalarını uygulayan bir C programıdır. Karate Club veri seti gibi sosyal ağ graflarında toplulukları tespit etmek için dört farklı algoritma kullanır.

## 📋 İçindekiler

- [Özellikler](#özellikler)
- [Kullanılan Algoritmalar](#kullanılan-algoritmalar)
- [Kurulum](#kurulum)
- [Kullanım](#kullanım)
- [Veri Formatı](#veri-formatı)
- [Çıktı Örneği](#çıktı-örneği)
- [Modularity (Q) Değeri](#modularity-q-değeri)

## 🎯 Özellikler

- **Louvain Algoritması**: İki aşamalı hiyerarşik topluluk tespiti
- **Fast Greedy Algoritması**: Açgözlü (greedy) topluluk birleştirme yaklaşımı
- **My Algorithm (Ortak Komşu Tabanlı)**: Ortak komşu sayısına göre topluluk ataması
- **My Algorithm 2 (Modularity Tabanlı)**: En çok komşu topluluk sayısına göre topluluk optimizasyonu
- **Modularity Hesaplama**: Her algoritmanın başarısını ölçmek için Q değeri hesaplama

## 🔧 Kullanılan Algoritmalar

### 1. Louvain Algoritması
- **Faz 1**: Her düğüm için modularity artışını maksimize eden topluluk ataması yapar
- **Faz 2**: Toplulukları tek düğüm gibi birleştirerek yeni bir graf oluşturur
- Bu işlem modularity değeri sabitlenene kadar tekrarlanır

### 2. Fast Greedy Algoritması
- En yüksek modularity artışı sağlayan topluluk çiftlerini birleştirir
- Artış kalmayıncaya kadar devam eder

### 3. My Algorithm (Ortak Komşu Tabanlı)
- Her düğüm, en fazla ortak komşuya sahip olduğu komşusunun topluluğuna geçer
- Topluluklar sabitlenene kadar iterasyon devam eder

### 4. My Algorithm 2 (Modularity Tabanlı)
- Her düğüm için komşu toplulukları sayar
- En çok komşu düğümünü içeren topluluğu seçer
- Modularity artışı sağlıyorsa topluluğu değiştirir
- Hiçbir düğüm değişmeyene kadar devam eder

## 📦 Kurulum

### Gereksinimler
- GCC veya herhangi bir C derleyicisi
- Windows, Linux veya macOS işletim sistemi

### Derleme

**Windows (GCC/MinGW):**
```bash
gcc proje.c -o a.exe
```

**Linux/macOS:**
```bash
gcc proje.c -o a.out
```

## 🚀 Kullanım

1. `data.txt` dosyasını proje dizinine yerleştirin
2. Programı derleyin
3. Programı çalıştırın:

**Windows:**
```bash
.\a.exe
```

**Linux/macOS:**
```bash
./a.out
```

## 📄 Veri Formatı

Veri dosyası (`data.txt`) aşağıdaki formatta olmalıdır:

```
[2 1]
[3 1] [3 2]
[4 1] [4 2] [4 3]
[5 1]
...
```

- Her satır bir veya daha fazla kenar içerir
- Format: `[düğüm1 düğüm2]`
- Düğümler 1'den başlar
- Graf yönsüzdür (undirected)

## 📊 Çıktı Örneği

```
Node : 34, Kenar : 78

deneme 1: Q : 0.381166, Community Sayisi= 3
Louvain node-community:
Community 0 : 1 2 3 4 5 6 7 8 10 11 12 13 14 17 18 20 22
Community 1 : 9 15 16 19 21 23 24 27 28 29 30 31 32 33 34
Community 2 : 25 26

 Fast Greedy 
Q : 0.380671, commun= 3
Fast Greedy node-community:
Community 0 : 1 5 6 7 11 12 17 20
Community 1 : 2 3 4 8 10 13 14 18 22
Community 8 : 9 15 16 19 21 23 24 25 26 27 28 29 30 31 32 33 34

 My Algorithm 
Q : 0.356016, commun= 3
My Algorithm node-community:
Community 1 : 1 2 3 4 5 6 7 8 10 11 12 13 14 17 18 20 22
Community 25 : 25 26
Community 33 : 9 15 16 19 21 23 24 27 28 29 30 31 32 33 34

 My Algorithm 2 
Q : 0.412, commun= 4
My Algorithm 2 node-community:
...
```

## 📈 Modularity (Q) Değeri

Modularity (Q), topluluk yapısının kalitesini ölçen bir metriktir:
- **Değer aralığı**: -0.5 ile 1.0 arasında
- **Yüksek Q değeri**: Güçlü topluluk yapısı
- **Düşük Q değeri**: Zayıf veya rastgele topluluk yapısı
- **Q ≈ 0.3-0.7**: Tipik sosyal ağlar için iyi değerler

### Formül:
$$Q = \frac{1}{2m} \sum_{i,j} \left[ A_{ij} - \frac{k_i k_j}{2m} \right] \delta(c_i, c_j)$$

Burada:
- $m$: Toplam kenar sayısı
- $A_{ij}$: Komşuluk matrisi
- $k_i, k_j$: Düğümlerin dereceleri
- $\delta(c_i, c_j)$: Aynı toplulukta ise 1, değilse 0

## 🏗️ Proje Yapısı

```
graph/
├── proje.c       # Ana kaynak kod
├── data.txt      # Veri dosyası (graf kenarları)
├── README.md     # Bu dosya
└── a.exe         # Derlenmiş program (Windows)
```

## 📚 Veri Yapısı

```c
typedef struct graph{
    int node_number;      // Düğüm sayısı
    int edge_number;      // Kenar sayısı
    int *community;       // Her düğümün topluluk kimliği
    int **matrix;         // Komşuluk matrisi
} graph;
```

## 🔍 Fonksiyonlar

### Temel Fonksiyonlar
- `connected_edge()`: İki düğüm arasında kenar kontrolü
- `expected_value()`: Beklenen kenar değeri hesaplama
- `modularity_network()`: Tüm ağ için modularity hesaplama
- `modularity_singular()`: Tek bir topluluk için modularity hesaplama

## 📝 Notlar

- Program, aynı veri setini dört farklı algoritmayla test eder
- Her algoritma için ayrı bir graf kopyası kullanılır
- Sonuçlar modularity değeriyle karşılaştırılabilir
- Louvain algoritması genellikle en yüksek Q değerini verir

