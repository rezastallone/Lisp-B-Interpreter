#include "tubesnew.h"

//fungsi mengecek apakah dia operator apa , atau dia unexpected token atau simbol yang gak bermakna apa apa untuk program kita misalnya ( * a 10 ) , a tidak bisa di kali 10 , nanti akan ada "unexpected token > a "
int precedence(string a)
{
    if (a == "*" || a == "/" || a == "%"  || a == "+" || a == "-"  || a == "exp" ) return 1;
    if (a == ">" || a == "<" || a == ">=" || a == "<=" || a == "=" || a == "/=" ) return 2;
    if (a == "AND" || a == "OR" || a == "NOT") return 3;

    for ( string::iterator it = a.begin() ; it != a.end() ; ++it ){
        if ( !isdigit(*it) &&  *it != '-' && *it != '.'  && *it != 'T' && *it != 'F'){
            cout <<"unexpected token > "<<*it<<endl;
            exit(2);        }
    }

}

//fungsi nge buat node , input nya isi tapi ada 4 case dari isi ini, kalau dia character "T" atau "F" artinya dia nampung nilai boolean , nah set boolcarriernya ke true nandain bahwa dia node pembawa boolean , kalau di test pake fungsi precedence yang di return antara 1 2 3 maka dia node pembawa operator ,maka set boolcarriernya ke false dulu, kalau dia bukan operator atau boolean maka dia angka biasa, set boolcarriernya ke false
tree* maketree(string isi)
{
    tree* baru = new tree;
    if ( isi == "T"){
        baru->nilai = true;
        baru->boolcarrier = true;
    }else if ( isi == "F" ){
        baru->nilai = false;
        baru->boolcarrier = true;
    }else if ( precedence(isi) < 4 ){
        baru->isi = isi;
        baru->boolcarrier = false;
    }else{
        baru->isi = isi;
        baru->value = atof(isi.c_str());
        baru->boolcarrier = false;
    }
    baru->left = NULL;
    baru->right = NULL;
    return baru;
}

//fungsi untuk evaluasi ekspresi
tree* evaluasi(tree* current, float& temp1 , float& temp2, bool& tempbool1 , bool& tempbool2){
    if ( precedence(current->isi) == 1 ){
        if ( current->right->boolcarrier || current->left->boolcarrier){ //mastin bahwa operator arithmatic gak nge evaulasi boolean
            cout <<"Unexpected boolean operand"<<endl;
            exit(3);
        }
        if ( current->isi == "*"){
        current->value = temp1 * temp2;
        }else if ( current->isi == "/"){
            current->value = temp1 / temp2;
        }else if ( current->isi == "%" ){
            current->value = float(int(temp1) % int(temp2));
        }else if ( current->isi == "+"){
            current->value = temp1 + temp2;
        }else if( current->isi == "-") {
            current->value = temp1 - temp2;
        }else if ( current->isi == "exp"){
            current->value = pow(temp1, temp2);
        }
        current->boolcarrier = false;
    }else if ( precedence(current->isi) == 2 ){
        if ( current->right->boolcarrier || current->left->boolcarrier ){ //mastiin bahwa operator relasional gak nge evaulasi boolean
            cout <<"Unexpected boolean operand"<<endl;
            exit(3);
        }
        if( current->isi == ">"){
            current->nilai = temp1 > temp2;
        }else if ( current->isi == "<" ){
            current->nilai = temp1 < temp2;
        }else if ( current->isi == ">=" ){
            current->nilai = temp1 >= temp2;
        }else if ( current->isi == "<=" ){
            current->nilai = temp1 <= temp2;
        }else if ( current->isi == "=" ){
            current->nilai = temp1 == temp2;
        }else if ( current->isi == "/=" ){
            current->nilai = temp1 != temp2;
        }
         current->boolcarrier = true;

    }else{
        if ( current->right->boolcarrier == false || current->left->boolcarrier == false){ // nge mastiin bahwa operator boolean gak nge evaulasi angka
            cout <<"Unexpected arithmatic operand"<<endl;
            exit(4);
        }
        if ( current->isi == "AND" ){
            current->nilai = tempbool1 && tempbool2;
        }else if ( current->isi == "OR"){
            current->nilai = tempbool1 || tempbool2;
        }else if ( current->isi == "NOT" ){
            current->nilai = ! tempbool1;
        }
        current->boolcarrier = true;
    }

    return current;
}

//algoritma buat sendiri , keren gak tuh ^_^V

//fungsi untuk parsing ekspresi dari array dengan index sebagai iterator
tree* mengisi(tree* current, string ekspresi[],int& index)
{
    current = maketree(ekspresi[index]); //pertama tama buat node
    index++;//tadi sudah baca operator , jadi di majukan iteratornya

    if ( ekspresi[index] == "(" ) // kalau dia kurung buka kita harus rekursif ke anak kiri
    {
        index++; // majukan karena sudah baca kurung buka
        current-> left = mengisi(current->left, ekspresi, index); //rekursif ke anak kiri
    }
    else
    {
        precedence(ekspresi[index]); //nge cek apakah unexpected token atau bukan
        current->left = maketree(ekspresi[index]); // kalau dia bukan kurung buka artinya dia langsung nilai murni, bisa angka atau boolean , biarkan fungsi maketree() yang menentukan
        index++; // majukan karena sudah baca
    }

    if ( ekspresi[index] == ")" ){ // ini nge handle kalau ekspresinya uner : (- -3)# atau (/ 10 )# atau ( NOT T ) #
        if ( current->isi == "-"){ //kalau uner nya - , maka isi anak kiri di kali -1 jadi di negativin
            current->right = maketree("-1");
            current->isi = "*";
        }else if ( current->isi == "/" ){ //kalau undernya / maka isi dari anak kiri kita tuker dengan yang kanan, lalu yang kiri di set satu jadi asalnya (/ 10)# menjadi ( / 1 10 )# hasilnya 0.1 gitu :D
            current->right = maketree(current->left->isi);
            current->left->value = 1;
        }else if ( current->isi == "NOT" ){ //uner boolean NOT , yang kiri sudah di dapat dari ekspresi (NOT T)# atau (NOT F)# ngisi yang kanan true aja , sebetulnya sih gak bakal ke pake tapi untuk jaga2 takut crash
            current->right = maketree("T");
        }
        index++; // maju karena sudah baca operator uner :D
        return current; //sung return karena uner langsung mentok ke kurung tutup :D
    }
    else if ( ekspresi[index] == "(" ) // kalau kurung buka kita rekursif ke anak kanan
    {
        index++;
        current->right = mengisi(current->right,ekspresi, index );
    }
    else
    {
        precedence(ekspresi[index]); //nge cek apakah unexpected token
        current->right = maketree(ekspresi[index]); // kalau dia bukan kurung buka artinya dia langsung nilai murni, bisa angka atau boolean , biarkan fungsi maketree() yang menentukan
        index++;
    }

    if ( ekspresi[index] != ")") //nge cek , jika ada kurung buka haruslah ada kurung tutup kalau tidak muncul expected token > )
    {
        cout <<"Expected token > )"<<endl;
        exit(1);
    }else{
        index++;
    }

    return current;
}

//fungsi untuk meng evaulasi ekspresi yang sudah di parsing di tree
tree* hasil(tree* current )
{
    //ke empat variabel ini akan di isi sesuai ekspresi
    float temp1;
    float temp2;
    bool tempbool1;
    bool tempbool2;

   //cout jangan di hapus , ini bug misterious

    //cout <<"";
    if ( precedence(current->left->isi) < 4  ) // kalau isi anak kiri itu operator , kita harus rekursif
    {
        current->left = hasil(current->left);
    }

    if ( current->left->boolcarrier){ // kalau bukan , cek apakah dia membawa nilai boolean
        tempbool1 = current->left->nilai; // kalau iya sung masukin ke penampung boolean yang dari anak kiri yaitu tempbool1 :D
    }
    temp1 = current->left->value; // kalau tidak maka dia membawa angka , sung masukin ke penampung angka dari anak kiri yaitu temp1 :D


    cout <<""; // jangna di hapus cout nya
    if ( precedence(current->right->isi) < 4  ) // sama dengan yang atas cuma untuk anak kanan
    {
        current->right = hasil(current->right);
    }

    if ( current->right->boolcarrier){
        tempbool2 = current->right->nilai;
    }
    temp2 = current->right->value;

    //cout <<"";
    return evaluasi(current, temp1, temp2, tempbool1, tempbool2); // setelah di dapat nilai nilai yang di perlukan untuk mengevaulasi , maka di masukan ke fungsi evaluasi tenang aja kalau ada variabel yang tidak terisi , karena akan ada handle nya atau tidak akan di pakai karena boolcarrier memastikan itu :D

}

/*
void ngetest(tree* current){

    if ( precedence(current->left->isi) > 0 && precedence(current->left->isi) < 4 ){
        cout <<" ( ";
        ngetest(current->left);
    }else if ( current->boolcarrier ){
        cout <<" ( "<<current->left->nilai;
    }else{
        cout <<" ( "<<current->left->value;
    }

    cout <<" "<<current->isi<<" ";

    if ( current->right != NULL ){

        if ( precedence(current->right->isi) > 0 && precedence(current->right->isi) < 4 ){
            ngetest(current->right);
            cout <<" ) ";
        }else if(current->right->boolcarrier){
            cout <<current->right->nilai<<" ) ";
        }else{
            cout <<current->right->value<<" ) ";
        }
    }

}
*/
