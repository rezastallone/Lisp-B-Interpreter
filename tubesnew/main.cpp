#include "tubesnew.h"

/** Author : Reza Stallone Asmuruf
contact : rezastallone@gmail.com
**/

int main()
{
    //kita meminta input dari user selama user tidak mengetik (exit) , kesalahan sintax atau program crash
    while ( true ) {
    vector<string> arith; //vektor itu struktur data kaya array tapi dinamis, kita gak harus tentuin ukurannya di awal , secara dinamis dia akan memperbesar sendiri kalo udah mentok , berguna untuk terima input user yang kita gak tau panjangnya berapa
    tree* arithTree = NULL; //binary tree ini yang akan nampung ekspresi kita nanti
    string line; //variabel pembantu untuk nampung input user
    int index = 0; //index iterasi nanti
    cout <<"> ";
    do{
        cin >> line;
        if ( line == "(exit)#"){
            return 0; //kalau line = (exit)# user mau keluar
        }else  if( line[line.find("(")] != string::npos && line != "("){ // if ini untuk nge handel kurung buka yang bersatu tanpa spasi dengan simbol lain contoh : 10(- ini di ubah jadi 10 ( - secara terpisah atau 10( jadi 10 ( atau (* jadi ( *
            int i = 0;
            for( string::iterator it = line.begin() ; it != line.end() ; ++it, ++i ){
                if ( ! isdigit(*it) && *it != '-' && *it != '.' && *it != 'e' && *it != 'x' && *it != 'p' ) {
                    if ( i != 0 ){
                        arith.push_back(line.substr(0, i));
                    }
                        arith.push_back(line.substr(i,1));
                        arith.push_back(line.substr(i+1));
                        break;
                }
            }
            continue;
        }else if ( line[line.find(")")] != string::npos && line != ")" ){ // if untuk nge handel kurung tutup yang bersatu tanpa spasi dengan simbol lain contoh : 10)# ini di ubah jadi 10 ) # secara terpisah atau )# jadi ) # atau 10) jadi 10 ) atau 10)))# jadi 10 ) ) ) #
            bool first = true;
            int i = 0;
            for( string::iterator it = line.begin() ; it != line.end() ; ++it, ++i ){
                if ( ! isdigit(*it) && *it != '-' && *it != '.'  && *it != 'e' && *it != 'x' && *it != 'p' ) {
                    if ( first && i != 0 ){
                        arith.push_back(line.substr(0, i));
                        first = false;
                    }
                        arith.push_back(line.substr(i,1));
                }
            }
            continue;
        }
        arith.push_back(line);
    }while ( line[line.find("#")] == string::npos ); // selama kita gak nemu # dalam inputan , kita bakal terus lanjut meski di pencet enter

    string ekspresi[arith.size()-2]; //pasring nanti menggunakan array, karena pake vektor error terus di library nya ga tau kenapa

    for ( int i = 0 ; i < arith.size()-2;i++ )
    {
        ekspresi[i] = arith[i+1]; // mindahin vektor ke array
    }

    arithTree = mengisi(arithTree, ekspresi, index); // di sini array di parsing ke tree

    //cout<<"\n";
    arithTree = hasil(arithTree); //tree di evaluasi dgn cara per level , enggak dengan inorder, postorder, atau preorder

    if ( arithTree->boolcarrier){ // di sini kita udah dapet hasil evaluasi , kalau boolcarrier root nya true , artinya hasil evaluasinya boolean
        if ( arithTree->nilai){
            cout <<"T"<<endl;
        }else{
            cout <<"F"<<endl;
        }
    }else{ // kalau nilai boolcarrier pada root false, artinya hasil evaluasi adalah angka
        cout <<arithTree->value<<endl;
    }

    //jaga jaga dan life cycle strukdatnya nya , kita bersih bersih :D
    delete arithTree;

    arithTree = NULL;

    arith.clear();

    }
}
