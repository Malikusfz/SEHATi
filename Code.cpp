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
    queue<string> patients; // Change from vector to queue
};

struct Patient
{
    int id;
    string name;
    int age;
    char gender;
    vector<int> doctorIds;
    bool isEmergency; // Tambahkan status khusus untuk pasien darurat
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
void addPatient(const string &name, int age, char gender, const vector<int> &doctorIds, bool isEmergency)
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
    newPatient.isEmergency = isEmergency;
    patients.push_back(newPatient);

    // Assign the patient to the corresponding doctors
    for (int doctorId : doctorIds)
    {
        for (Doctor &doctor : doctors)
        {
            if (doctor.id == doctorId)
            {
                doctor.patients.push(name); // Use push instead of push_back
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
void removePatient(int patientId)
{
    patients.erase(
        remove_if(patients.begin(), patients.end(),
            [&](const Patient &patient)
            { return patient.id == patientId; }),
        patients.end());

    // Remove the patient from the assigned doctors
    for (Doctor &doctor : doctors)
    {
        // Find the patient in the queue and remove them
        queue<string> tempQueue;
        while (!doctor.patients.empty())
        {
            string patientName = doctor.patients.front();
            doctor.patients.pop();
            auto patientIt = find_if(patients.begin(), patients.end(),
                [&](const Patient &patient)
                { return patient.name == patientName; });
            if (patientIt == patients.end() || patientIt->id == patientId)
            {
                continue; // Skip this patient
            }
            tempQueue.push(patientName);
        }
        swap(doctor.patients, tempQueue);
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

 // Sort patients, putting emergency patients at the beginning
 auto comparePatients = [](const Patient &a, const Patient &b)
 {
 if (a.isEmergency && !b.isEmergency)
 {
 return true;
 }
 else if (!a.isEmergency && b.isEmergency)
 {
 return false;
 }
 else
 {
 return a.id < b.id; // Sort non-emergency patients by ID
 }
 };

 vector<Patient> sortedPatients = patients;
 sort(sortedPatients.begin(), sortedPatients.end(), comparePatients);

 for (const Patient &patient : sortedPatients)
 {
 cout << "ID Pasien: " << patient.id << " || Nama: " << patient.name << " || Umur: " << patient.age << " || Kelamin: " << patient.gender << " || Dokter: ";
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

 if (patient.isEmergency)
 {
 cout << "(Pasien Darurat)";
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
            cout << "Pasien yang ditangani oleh Dokter: " << doctor.name << "\n"
                << "Banyaknya: " << doctor.patients.size() << "\n";

            // Create a vector of patients handled by this doctor
            vector<Patient> patientsByDoctor;
            queue<string> tempQueue = doctor.patients; // Copy the queue to a temporary queue
            while (!tempQueue.empty())
            {
                string patientName = tempQueue.front();
                tempQueue.pop();
                auto patientIt = find_if(patients.begin(), patients.end(), [&](const Patient &patient)
                    { return patient.name == patientName; });
                if (patientIt != patients.end())
                {
                    patientsByDoctor.push_back(*patientIt);
                }
            }

            // Sort the vector of patients, putting emergency patients at the beginning
            sort(patientsByDoctor.begin(), patientsByDoctor.end(), [](const Patient &a, const Patient &b)
                {
                    if (a.isEmergency && !b.isEmergency) {
                        return true;
                    }
                    else if (!a.isEmergency && b.isEmergency) {
                        return false;
                    }
                    else {
                        return a.id < b.id; // Sort non-emergency patients by ID
                    }
                });

            // Display the sorted list of patients
            for (const Patient &patient : patientsByDoctor)
            {
                cout << "ID Pasien: " << patient.id << " || Nama: " << patient.name;
                if (patient.isEmergency)
                {
                    cout << " (Pasien Darurat)";
                }
                cout << endl;
            }

            break;
        }
    }
    if (!doctorFound)
    {
        cout << "Dokter dengan ID " << doctorId << " tidak ditemukan.\n";
    }
}

void removePatientByDoctorQueue(int doctorId)
{
    // Find the doctor
    auto doctorIt = find_if(doctors.begin(), doctors.end(),
        [&](const Doctor &doctor)
        { return doctor.id == doctorId; });

    if (doctorIt != doctors.end())
    {
        // Check if the doctor has any patients
        if (!doctorIt->patients.empty())
        {
            // Find the first emergency patient
            string emergencyPatientName;
            bool emergencyPatientFound = false;
            queue<string> tempQueue; // Temporary queue to store non-emergency patients
            while (!doctorIt->patients.empty())
            {
                string patientName = doctorIt->patients.front();
                doctorIt->patients.pop();
                auto patientIt = find_if(patients.begin(), patients.end(),
                    [&](const Patient &patient)
                    { return patient.name == patientName; });
                if (patientIt != patients.end() && patientIt->isEmergency)
                {
                    emergencyPatientName = patientName;
                                        emergencyPatientFound = true;
                    break;
                }
                else
                {
                    tempQueue.push(patientName);
                }
            }

            // Put the non-emergency patients back in the doctor's queue
            while (!tempQueue.empty())
            {
                doctorIt->patients.push(tempQueue.front());
                tempQueue.pop();
            }

            // If an emergency patient is found, remove them
            if (emergencyPatientFound)
            {
                // Find the patient by name
                auto patientIt = find_if(patients.begin(), patients.end(),
                    [&](const Patient &patient)
                    { return patient.name == emergencyPatientName; });

                if (patientIt != patients.end())
                {
                    int patientId = patientIt->id;

                    // Remove the patient from the overall patient list
                    removePatient(patientId);
                }
            }
            else // If no emergency patient is found, remove the first non-emergency patient
            {
                const string &patientName = doctorIt->patients.front();
                doctorIt->patients.pop();

                // Find the patient by name
                auto patientIt = find_if(patients.begin(), patients.end(),
                    [&](const Patient &patient)
                    { return patient.name == patientName; });

                if (patientIt != patients.end())
                {
                    int patientId = patientIt->id;

                    // Remove the patient from the overall patient list
                    removePatient(patientId);
                }
            }
        }
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
        emergencyPatient.isEmergency = true; // Set status pasien darurat

        // Tambahkan pasien darurat ke daftar pasien
        patients.push_back(emergencyPatient);

        // Tambahkan pasien darurat ke dokter yang menanganinya
        minPatientsDoctor->patients.push(emergencyPatientName); // Use push instead of push_back

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
            queue<string> tempQueue; // Temporary queue to store non-matching patients
            while (!oldDoctorIt->patients.empty())
            {
                string currentPatientName = oldDoctorIt->patients.front();
                oldDoctorIt->patients.pop();
                if (currentPatientName != patientName)
                {
                    tempQueue.push(currentPatientName);
                }
            }
            swap(oldDoctorIt->patients, tempQueue);
        }

        // Add the patient to the new doctor's list
        auto newDoctorIt = find_if(doctors.begin(), doctors.end(),
            [&](const Doctor &doctor)
            { return doctor.id == newDoctorId; });
        if (newDoctorIt != doctors.end())
        {
            newDoctorIt->patients.push(patientName); // Use push instead of push_back
        }
    }
}

int main()
{
    int choice, Patientid, doctorId;
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
        cout << "10. Hapus Pasien Pasca-Konsultasi\n";
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
            addPatient(patientName, age, gender, doctorIds, false);
            cin.ignore(); // Wait for the user to press enter
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
            cin.ignore();
        }
        else if (choice == 4)
        {
            showPatientList();
            cout << "ID Pasien yang akan dihapus: ";
            cin >> Patientid;
            removePatient(Patientid);
            cout << "Data pasien berhasil dihapus.\n";
            cin.ignore();
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
            cin.ignore();
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
                        queue<string> tempQueue; // Temporary queue to store non-matching patients
                        while (!oldDoctorIt->patients.empty())
                        {
                            string currentPatientName = oldDoctorIt->patients.front();
                            oldDoctorIt->patients.pop();
                            if (currentPatientName != patientIt->name)
                            {
                                tempQueue.push(currentPatientName);
                            }
                        }
                        swap(oldDoctorIt->patients, tempQueue);
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
                    newDoctorIt->patients.push(patientIt->name); // Use push instead of push_back
                }
            }
            else
            {
                cout << "Pasien dengan ID " << patientId << " tidak ditemukan.\n";
            }
        }
        else if (choice == 10)
        {
            displayDatadoc();
            cout << "ID Dokter: ";
            cin >> doctorId;
            removePatientByDoctorQueue(doctorId);
            cin.ignore();
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



