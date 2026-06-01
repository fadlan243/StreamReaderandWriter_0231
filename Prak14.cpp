#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <stdexcept>
#include <vector>

using namespace std;

// =========================================================================
// 1. STRUKTUR KELAS TOKO ELEKTRONIK (ETALASE & EXCEPTION HANDLING)
// =========================================================================
class TokoElektronik {
private:
    // Menggunakan std::array kapasitas 3 elemen untuk mendukung fungsi .at()
    array<string, 3> etalase;

public:
    // Constructor untuk mengisi data awal secara otomatis
   

    // Method untuk mengambil produk berdasarkan nomorRak dengan Exception Handling
    string ambilProduk(size_t nomorRak) {
        try {
            // Wajib menggunakan .at() untuk mengecek batas indeks array
            return etalase.at(nomorRak);
        }
        catch (const out_of_range& e) {
            // Melemparkan kembali (throw) pesan error kustom sesuai spesifikasi
            throw runtime_error("Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!");
        }
    }
};

// =========================================================================
// 3. FITUR MANAJEMEN GUDANG BERBASIS FILE (CRUD BERBASIS FILE)
// =========================================================================
const string FILE_NAME = "gudang.txt";

// Struktur helper untuk mempermudah manipulasi data barang di memori
struct Barang {
    string id;
    string nama;
    string harga;
};

// Fungsi Helper untuk membaca semua data dari file ke Vector
vector<Barang> bacaSemuaBarang() {
    vector<Barang> daftarBarang;
    ifstream file(FILE_NAME);
    
    if (!file.is_open()) {
        return daftarBarang; // Kembalikan vector kosong jika file belum ada
    }

    Barang b;
    // Membaca file dengan format per baris: ID, Nama, Harga
    while (getline(file, b.id) && getline(file, b.nama) && getline(file, b.harga)) {
        daftarBarang.push_back(b);
    }
    file.close();
    return daftarBarang;
}

// Fungsi Helper untuk menulis ulang seluruh Vector ke File
void tulisSemuaBarang(const vector<Barang>& daftarBarang) {
    ofstream file(FILE_NAME, ios::trunc); // Menimpa file lama
    if (file.is_open()) {
        for (const auto& b : daftarBarang) {
            file << b.id << "\n" << b.nama << "\n" << b.harga << "\n";
        }
        file.close();
    }
}

// --- Operasi READ ---
void tampilkanGudang() {
    vector<Barang> daftarBarang = bacaSemuaBarang();
    
    cout << "\n==================================================\n";
    cout << "               DAFTAR BARANG DI GUDANG            \n";
    cout << "==================================================\n";
    if (daftarBarang.empty()) {
        cout << " [ Data gudang kosong atau file belum dibuat ]\n";
    } else {
        cout << "ID\t|\tNama Barang\t\t|\tHarga\n";
        cout << "--------------------------------------------------\n";
        for (const auto& b : daftarBarang) {
            cout << b.id << "\t|\t" << b.nama << "\t\t|\tRp. " << b.harga << "\n";
        }
    }
    cout << "==================================================\n";
}

// --- Operasi CREATE ---
void tambahBarang() {
    Barang b;
    cin.ignore();
    cout << "\n--- Tambah Barang Baru ---\n";
    cout << "Masukkan ID Barang   : "; getline(cin, b.id);
    cout << "Masukkan Nama Barang : "; getline(cin, b.nama);
    cout << "Masukkan Harga       : "; getline(cin, b.harga);

    // Buka file dengan mode append (menambahkan di akhir)
    ofstream file(FILE_NAME, ios::app);
    if (file.is_open()) {
        file << b.id << "\n" << b.nama << "\n" << b.harga << "\n";
        file.close();
        cout << "\n>> Berhasil! Barang ditambahkan ke gudang.txt <<\n";
    } else {
        cout << "\n>> Gagal membuka file untuk menulis data! <<\n";
    }
}

// --- Operasi UPDATE ---
void updateBarang() {
    string idCari;
    cin.ignore();
    cout << "\n--- Update Data Barang ---\n";
    cout << "Masukkan ID Barang yang ingin diubah: "; getline(cin, idCari);

    vector<Barang> daftarBarang = bacaSemuaBarang();
    bool ditemukan = false;

    for (auto& b : daftarBarang) {
        if (b.id == idCari) {
            ditemukan = true;
            cout << "Data Lama -> " << b.nama << " (Rp. " << b.harga << ")\n";
            cout << "Masukkan Nama Baru : "; getline(cin, b.nama);
            cout << "Masukkan Harga Baru: "; getline(cin, b.harga);
            break;
        }
    }

    if (ditemukan) {
        tulisSemuaBarang(daftarBarang);
        cout << "\n>> Berhasil! Data barang telah diperbarui di file. <<\n";
    } else {
        cout << "\n>> Gagal! Barang dengan ID [" << idCari << "] tidak ditemukan. <<\n";
    }
}

// --- Operasi DELETE ---
void hapusBarang() {
    string idCari;
    cin.ignore();
    cout << "\n--- Hapus Data Barang ---\n";
    cout << "Masukkan ID Barang yang ingin dihapus: "; getline(cin, idCari);

    vector<Barang> daftarBarang = bacaSemuaBarang();
    bool ditemukan = false;

    for (auto it = daftarBarang.begin(); it != daftarBarang.end(); ++it) {
        if (it->id == idCari) {
            daftarBarang.erase(it);
            ditemukan = true;
            break;
        }
    }

    if (ditemukan) {
        tulisSemuaBarang(daftarBarang);
        cout << "\n>> Berhasil! Barang telah dihapus dari file. <<\n";
    } else {
        cout << "\n>> Gagal! Barang dengan ID [" << idCari << "] tidak ditemukan. <<\n";
    }
}

// =========================================================================
// SIMULASI ETALASE (PENGUJIAN EXCEPTION HANDLING POIN 2 & 4)
// =========================================================================
void jalankanSimulasiEtalase(TokoElektronik& toko) {
    cout << "\n==================================================\n";
    cout << "           SIMULASI PENGAMBILAN ETALASE           \n";
    cout << "==================================================\n";

    // --- Skenario 1: Pengambilan barang di rak indeks ke-1 (Harus sukses) ---
    cout << "[Skenario 1] Mengambil barang di rak indeks 1...\n";
    try {
        string barang = toko.ambilProduk(1);
        cout << "Hasil Skenario 1 -> Sukses! Barang ditemukan: " << barang << "\n";
    }
    catch (const runtime_error& e) {
        cout << "Hasil Skenario 1 -> Terjadi Error: " << e.what() << "\n";
    }

    cout << "--------------------------------------------------\n";

    // --- Skenario 2: Pengambilan barang di rak indeks ke-5 (Harus gagal) ---
    cout << "[Skenario 2] Mengambil barang di rak indeks 5...\n";
    try {
        string barang = toko.ambilProduk(5);
        cout << "Hasil Skenario 2 -> Sukses! Barang ditemukan: " << barang << "\n";
    }
    catch (const runtime_error& e) {
        // Menangkap pesan error kustom yang dilemparkan ulang oleh method ambilProduk
        cout << "Hasil Skenario 2 -> Terjadi Error:\n";
        cout << ">>> " << e.what() << " <<<\n";
    }
    cout << "==================================================\n";
}

// =========================================================================
// 4. PENGUJIAN PADA FUNGSI MAIN()
// =========================================================================
int main() {
    TokoElektronik gibranJaya;
    int pilihanMenu;

    do {
        // Menampilkan daftar barang dari file secara otomatis saat menu dibuka (Sesuai Poin 3 - Read)
        tampilkanGudang();

        // Tampilan Menu Utama CLI
        cout << "\n=== SISTEM DIGITALISASI TOKO \"GIBRAN JAYA\" ===\n";
        cout << "1. Tambah Barang Baru ke Gudang (Create)\n";
        cout << "2. Update Data Barang Gudang (Update)\n";
        cout << "3. Hapus Data Barang Gudang (Delete)\n";
        cout << "4. Jalankan Simulasi Etalase (Uji Exception Handling)\n";
        cout << "5. Keluar Aplikasi\n";
        cout << "Pilih menu (1-5): ";
        cin >> pilihanMenu;

        // Validasi input jika user memasukkan selain angka
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n>> Input tidak valid! Masukkan angka antara 1-5. <<\n";
            continue;
        }

        switch (pilihanMenu) {
            case 1:
                tambahBarang();
                break;
            case 2:
                updateBarang();
                break;
            case 3:
                hapusBarang();
                break;
            case 4:
                jalankanSimulasiEtalase(gibranJaya);
                break;
            case 5:
                cout << "\nTerima kasih telah menggunakan sistem manajemen Gibran Jaya!\n";
                break;
            default:
                cout << "\n>> Pilihan menu tidak tersedia! Silakan coba lagi. <<\n";
        }
        
        cout << "\nTekan Enter untuk melanjutkan...";
        cin.get(); 

    } while (pilihanMenu != 5);

    return 0;
}