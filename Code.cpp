#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using namespace std;

struct Doctor
{
    int id;
    string name;
    string specialist;
    vector<string> patients;
};

struct Patient
{
    int id;
    string name;
    int age;
    char gender;
    vector<int> doctorIds;
};

vector<Doctor> doctors;
vector<Patient> patients;
int doctorIdCounter = 1;
int patientIdCounter = 1;

// Function to add a new doctor
void addDoctor(const string &name, const string &specialist)
{
    Doctor newDoctor;
    newDoctor.id = doctorIdCounter++;
    newDoctor.name = name;
    newDoctor.specialist = specialist;
    doctors.push_back(newDoctor);
}

// Function to add a new patient
void addPatient(const string &name, int age, char gender, const vector<int> &doctorIds)
{
    // Check if the gender is valid
    if (gender != 'L' && gender != 'P')
    {
        cout << "Error: Kelamin pasien tidak valid.\n";
        return;
    }

    Patient newPatient;
    newPatient.id = patientIdCounter++;
    newPatient.name = name;
    newPatient.age = age;
    newPatient.gender = gender;
    newPatient.doctorIds = doctorIds;
    patients.push_back(newPatient);

    // Assign the patient to the corresponding doctors
    for (int doctorId : doctorIds)
    {
        for (Doctor &doctor : doctors)
        {
            if (doctor.id == doctorId)
            {
                doctor.patients.push_back(name);
                break;
            }
        }
    }
}

// Function to remove a doctor
void removeDoctor(int doctorId)
{
    doctors.erase(
        remove_if(doctors.begin(), doctors.end(),
                  [&](const Doctor &doctor)
                  { return doctor.id == doctorId; }),
        doctors.end());

    // Remove the doctor from the assigned patients
    for (Patient &patient : patients)
    {
        patient.doctorIds.erase(
            remove(patient.doctorIds.begin(), patient.doctorIds.end(), doctorId),
            patient.doctorIds.end());
    }
}

// Function to remove a patient
void removePatient(const string &name)
{
    patients.erase(
        remove_if(patients.begin(), patients.end(),
                  [&](const Patient &patient)
                  { return patient.name == name; }),
        patients.end());

    // Remove the patient from the assigned doctors
    for (Doctor &doctor : doctors)
    {
        doctor.patients.erase(
            remove(doctor.patients.begin(), doctor.patients.end(), name),
            doctor.patients.end());
    }
}

void displayDatadoc()
{
    if (doctors.empty())
    {
        cout << "Tidak ada data dokter saat ini.\n";
    }
    else
    {
        cout << "Data Dokter:\n";
        for (const Doctor &doctor : doctors)
        {
            cout << "ID Dokter: " << doctor.id << " || Nama: " << doctor.name << " || Spesialis: " << doctor.specialist << " || Jumlah Pasien: " << doctor.patients.size() << endl;
        }
    }
}

// Function to show the list of patients
void showPatientList()
{
    if (patients.empty())
    {
        cout << "Tidak ada pasien saat ini.\n";
    }
    else
    {
        cout << "Daftar Pasien:\n";
        for (const Patient &patient : patients)
        {
            cout << "ID Pasien: " << patient.id << "|| Nama: " << patient.name << " || Umur: " << patient.age << " || Kelamin: " << patient.gender << " || Dokter: ";
            for (int doctorId : patient.doctorIds)
            {
                for (const Doctor &doctor : doctors)
                {
                    if (doctor.id == doctorId)
                    {
                        cout << doctor.name << " ";
                        break;
                    }
                }
            }
            cout << endl;
        }
    }
}

// Function to display all data
void displayData()
{
    displayDatadoc();
    showPatientList();
}

// Function to display and sort doctors based on the number of patients they have
void displaySortedDoctors()
{
    vector<Doctor> sortedDoctors = doctors;

    sort(sortedDoctors.begin(), sortedDoctors.end(),
         [](const Doctor &a, const Doctor &b)
         { return a.patients.size() > b.patients.size(); });
    
    cout << "Daftar Dokter (urut berdasarkan jumlah pasien):\n";
    for (const Doctor &doctor : sortedDoctors)
    {
        cout << "ID: " << doctor.id << " || Nama: " << doctor.name << " || Spesialis: " << doctor.specialist << " || Jumlah Pasien: " << doctor.patients.size() << endl;
    }
}

// Function to find patients by doctor
void findPatientsByDoctor(const int doctorId)
{
    bool doctorFound = false;
    for (const Doctor &doctor : doctors)
    {
        if (doctor.id == doctorId)
        {
            doctorFound = true;
            system("cls");
            /* cout << "Banyak nya pasien yang di tanganin oleh dokter " << doctor.id << ": " << doctor.patients.size() << endl; // Debugging statement */
            cout << "Pasien yang ditangani oleh Dokter: " << doctor.name <<"\n" << "Banyaknya: "<< doctor.patients.size() << "\n";
            for (const string &patient : doctor.patients)
            {
                cout << patient << endl;
            }
            break;
        }
    }

    if (!doctorFound)
    {
        cout << "Dokter dengan ID " << doctorId << " tidak ditemukan.\n";
    }
}

// Function to handle emergency patients
void handleEmergencyPatient()
{
    
        // Find the doctor with the fewest patients
        auto minPatientsDoctor = min_element(doctors.begin(), doctors.end(),
                                             [](const Doctor &a, const Doctor &b)
                                             {
                                                 return a.patients.size() < b.patients.size();
                                             });

        if (minPatientsDoctor != doctors.end())
        {
            // Input data pasien darurat
            string emergencyPatientName;
            int emergencyPatientAge;
            char emergencyPatientGender;

            cout << "Masukkan data pasien darurat:\n";
            cout << "Nama: ";
            cin.ignore();
            getline(cin, emergencyPatientName);
            cout << "Umur: ";
            cin >> emergencyPatientAge;
            cout << "Kelamin (L/P): ";
            cin >> emergencyPatientGender;

            // Buat objek pasien darurat
            Patient emergencyPatient;
            emergencyPatient.id = patientIdCounter++;
            emergencyPatient.name = emergencyPatientName;
            emergencyPatient.age = emergencyPatientAge;
            emergencyPatient.gender = emergencyPatientGender;
            emergencyPatient.doctorIds.push_back(minPatientsDoctor->id);

            // Tambahkan pasien darurat ke daftar pasien
            patients.push_back(emergencyPatient);

            // Tambahkan pasien darurat ke dokter yang menanganinya
            minPatientsDoctor->patients.push_back(emergencyPatientName);

            cout << "Pasien darurat " << emergencyPatientName << " ditangani oleh dokter " << minPatientsDoctor->name << endl;
        }
        else
        {
            cout << "Tidak ada dokter yang tersedia saat ini.\n";
        }
    
}


void changeDoctor(const string &patientName, int oldDoctorId, int newDoctorId)
{
    // Find the patient
    auto patientIt = find_if(patients.begin(), patients.end(),
                             [&](const Patient &patient)
                             { return patient.name == patientName; });

    if (patientIt != patients.end())
    {
        // Remove the old doctor from the patient's list
        patientIt->doctorIds.erase(
            remove(patientIt->doctorIds.begin(), patientIt->doctorIds.end(), oldDoctorId),
            patientIt->doctorIds.end());

        // Add the new doctor to the patient's list
        patientIt->doctorIds.push_back(newDoctorId);

        // Remove the patient from the old doctor's list
        auto oldDoctorIt = find_if(doctors.begin(), doctors.end(),
                                   [&](const Doctor &doctor)
                                   { return doctor.id == oldDoctorId; });
        if (oldDoctorIt != doctors.end())
        {
            oldDoctorIt->patients.erase(
                remove(oldDoctorIt->patients.begin(), oldDoctorIt->patients.end(), patientName),
                oldDoctorIt->patients.end());
        }

        // Add the patient to the new doctor's list
        auto newDoctorIt = find_if(doctors.begin(), doctors.end(),
                                   [&](const Doctor &doctor)
                                   { return doctor.id == newDoctorId; });
        if (newDoctorIt != doctors.end())
        {
            newDoctorIt->patients.push_back(patientName);
        }
    }
}

int main()
{
    int choice;
    string doctorName, specialist, patientName;
    int age;
    char gender;

    do
    {
        cout << "\n";
        cout << "Sistem Informasi Rumah Sakit SEHATi\n";
        cout << "=======================================================\n";
        cout << "1. Tambah Dokter\n";
        cout << "2. Tambah Pasien\n";
        cout << "3. Hapus Dokter\n";
        cout << "4. Hapus Pasien\n";
        cout << "5. Tampilkan Data\n";
        cout << "6. Tampilkan Dokter (urut berdasarkan jumlah pasien)\n";
        cout << "7. Cari Pasien berdasarkan Dokter\n";
        cout << "8. Tangani Pasien Darurat\n";
        cout << "9. Edit Relasi Dokter-Pasien\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer

        if (choice == 1)
        {
            displayDatadoc();
            cout << "Nama Dokter: ";
            getline(cin, doctorName);
            cout << "Spesialis: ";
            getline(cin, specialist);
            addDoctor(doctorName, specialist);
            cout << "Data dokter berhasil ditambahkan.\n";
        }
        else if (choice == 2)
        {
            showPatientList();
            cout << "Nama Pasien: ";
            getline(cin, patientName);
            cout << "Umur: ";
            cin >> age;
            cin.ignore(); // Clear the newline character from the input buffer
            cout << "Kelamin (L/P): ";
            cin >> gender;
            displayDatadoc();
            cout << "ID Dokter yang menangani: ";
            int doctorId;
            cin >> doctorId;
            vector<int> doctorIds(1, doctorId); // Create a vector with a single element
            addPatient(patientName, age, gender, doctorIds);
            cout << "Data pasien berhasil ditambahkan.\n";
        }

        else if (choice == 3)
        {
            displayDatadoc();
            cout << "ID Dokter yang akan dihapus: ";
            int doctorId;
            cin >> doctorId;
            removeDoctor(doctorId);
            cout << "Data dokter berhasil dihapus.\n";
        }
        else if (choice == 4)
        {
            showPatientList();
            cout << "Nama Pasien yang akan dihapus: ";
            getline(cin, patientName);
            removePatient(patientName);
            cout << "Data pasien berhasil dihapus.\n";
        }
        else if (choice == 5)
        {
            displayData();
        }
        else if (choice == 6)
        {
            displaySortedDoctors();
        }
        else if (choice == 7)
        {
            displayDatadoc();
            cout << "ID Dokter: ";
            int doctorId;
            cin >> doctorId;
            cin.ignore(); // Clear the newline character from the input buffer
            findPatientsByDoctor(doctorId);
        }

        else if (choice == 8)
        {
            handleEmergencyPatient();
        }
        else if (choice == 9)
        {
            showPatientList();
            cout << "ID Pasien: ";
            int patientId;
            cin >> patientId;
            cin.ignore(); // Clear the newline character from the input buffer

            // Find the patient with the specified ID
            auto patientIt = find_if(patients.begin(), patients.end(),
                                     [&](const Patient &patient)
                                     { return patient.id == patientId; });

            if (patientIt != patients.end())
            {

                cout << "Nama Pasien: " << patientIt->name << endl;
                displayDatadoc();
                cout << "ID Dokter Baru: ";
                int newDoctorId;
                cin >> newDoctorId;
                cin.ignore(); // Clear the newline character from the input buffer

                // Remove the patient from all current doctors
                for (int oldDoctorId : patientIt->doctorIds)
                {
                    auto oldDoctorIt = find_if(doctors.begin(), doctors.end(),
                                               [&](const Doctor &doctor)
                                               { return doctor.id == oldDoctorId; });
                    if (oldDoctorIt != doctors.end())
                    {
                        oldDoctorIt->patients.erase(
                            remove(oldDoctorIt->patients.begin(), oldDoctorIt->patients.end(), patientIt->name),
                            oldDoctorIt->patients.end());
                    }
                }

                // Clear the patient's list of doctors
                patientIt->doctorIds.clear();

                // Add the new doctor to the patient's list
                patientIt->doctorIds.push_back(newDoctorId);

                // Add the patient to the new doctor's list
                auto newDoctorIt = find_if(doctors.begin(), doctors.end(),
                                           [&](const Doctor &doctor)
                                           { return doctor.id == newDoctorId; });
                if (newDoctorIt != doctors.end())
                {
                    newDoctorIt->patients.push_back(patientIt->name);
                }
            }
            else
            {
                cout << "Pasien dengan ID " << patientId << " tidak ditemukan.\n";
            }
        }

        else if (choice == 0)
        {
            cout << "Terima kasih telah menggunakan aplikasi ini.\n";
        }
        else
        {
            cout << "Pilihan tidak valid.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore(); // Wait for the user to press enter
        cout << endl;
        system("cls");

    } while (choice != 0);

    return 0;
}
