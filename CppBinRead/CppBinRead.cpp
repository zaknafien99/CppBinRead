#include "pch.h"

using namespace System;
using namespace System::IO;

// The Customer class
ref class Customer
{
    String^ name;
    long accNo;
    double balance;

public:
    // Default constructor
    Customer() : name(nullptr), accNo(0), balance(0.0) {}

    Customer(String ^n, long ac, double bal) : name(n), accNo(ac), balance(bal)
    {}

    // Properties to retrieve instance data
    property String^ Name
    {
        String^ get() { return name; }
    }

    property long AccountNumber
    {
        long get() { return accNo; }
    }

    property double Balance
    {
        double get() { return balance; }
    }

    // Write object
    void Write(BinaryWriter^ bw)
    {
        bw->Write(name);
        bw->Write(accNo);
        bw->Write(balance);
    }

    // Read object
    void Read(BinaryReader^ br)
    {
        name = br->ReadString();
        accNo = br->ReadInt32();
        balance = br->ReadDouble();
    }
};

int main(array<System::String ^> ^args)
{
    if (args->Length == 0)
    {
        Console::WriteLine("Usage: CppBinRead [path]");
        return 0;
    }

    String^ path = args[0];

    // Create some customers
    Customer^ c1 = gcnew Customer("Fred Smith", 1234567, 100.0);
    Customer^ c2 = gcnew Customer("Jane Doe", 2345678, 1000.0);
    Customer^ c3 = gcnew Customer("Gill Evans", 3456789, 500.0);

    FileStream^ fs = nullptr;
    try
    {
        // Create a FileStream to write to the file
        fs = gcnew FileStream(path, FileMode::Create, FileAccess::ReadWrite);

        // Create a BinaryWriter
        BinaryWriter^ bw = gcnew BinaryWriter(fs);

        //Write the objects to the file
        c1->Write(bw);
        c2->Write(bw);
        c3->Write(bw);

        // Create a BinaryReader that reads from the same FileStream
        BinaryReader^ br = gcnew BinaryReader(fs);

        // Move back to the beginning
        br->BaseStream->Seek(0, SeekOrigin::Begin);

        Customer^ c4 = gcnew Customer();
        c4->Read(br);
        Console::WriteLine("Balance for {0} (a/c {1}) is {2}", c4->Name, c4->AccountNumber, c4->Balance);
    }
    catch (IOException^ iex)
    {
        Console::WriteLine(iex->Message);
        return -1;
    }
    finally
    {
        if (fs != nullptr) fs->Close();
    }
    return 0;
}
