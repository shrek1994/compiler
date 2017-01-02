#include "Interpreter.hpp"
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include "debug.hpp"

#include<cstdlib> 	// rand()
#include<ctime>

int Interpreter::run(std::istream &plik, std::istream &cin, std::ostream &cout, std::ostream &info)
{
    using std::endl;
    using std::get;
    std::vector< std::tuple<Instructions,int,int> > program;
    std::map<int,long long> pam;

    constexpr int reg=5;
    long long r[reg];
    int lr;

    int k=0;
    long long i;
    Instructions i1;
    int i2, i3;
    std::string com;

    while( !plik.eof() )
    {
        plik >> com;
        i1 = ERROR;
        i2 = 0;
        i3 = 0;

        if( com=="GET" ) { i1 = GET; plik >> i2; }
        if( com=="PUT" ) { i1 = PUT; plik >> i2; }

        if( com=="LOAD"  ) { i1 = LOAD; plik >> i2; }
        if( com=="STORE" ) { i1 = STORE; plik >> i2; }

        if( com=="COPY" ) { i1 = COPY; plik >> i2; }
        if( com=="ADD"  ) { i1 = ADD; plik >> i2; }
        if( com=="SUB"  ) { i1 = SUB; plik >> i2; }
        if( com=="SHR"  ) { i1 = SHR; plik >> i2; }
        if( com=="SHL"  ) { i1 = SHL; plik >> i2; }
        if( com=="INC"  ) { i1 = INC; plik >> i2; }
        if( com=="DEC"  ) { i1 = DEC; plik >> i2; }
        if( com=="ZERO" ) { i1 = ZERO; plik >> i2; }

        if( com=="JUMP"  ) { i1 = JUMP; plik >> i3; }
        if( com=="JZERO" ) { i1 = JZERO; plik >> i2;  plik >> i3; }
        if( com=="JODD"  ) { i1 = JODD; plik >> i2;  plik >> i3; }
        if( com=="HALT"  ) { i1 = HALT; }

        if( i1==ERROR ) { cout << "Blad: Nieznana instrukcja w linii " << k << " (" << com <<")." << endl; return -1; }
        if( i2>reg-1 ) { cout << "Blad: zly rejestr w instrukcji w linii " << k << " (" << i2 <<")."<< endl; return -1; }
        if( i2<0 ) { cout << "Blad: Zly rejestr w instrukcji w linii " << k <<  " (" << i2 << ")." << endl; return -1; }
        if( i3<0 ) { cout << "Blad: Zly adress w instrukcji w linii " << k <<  " (" << i3 << ")." << endl; return -1; }

        if( plik.good() )
        {
            program.push_back( std::make_tuple(i1,i2,i3) );
        }
        k++;
    }

    DEBUG << "Skończono czytanie pliku (" << program.size() << " linii)." << endl;
    info << "Skończono czytanie pliku (" << program.size() << " linii)." << endl;

    info << "Uruchamianie programu." << endl;
    lr = 0;
    std::srand(std::time(NULL));
    for(int i = 0; i<reg; i++ ) r[i] = std::rand();
    i = 0;
    while( get<0>(program[lr])!=HALT )	// HALT
    {
        switch( get<0>(program[lr]) )
        {
        case GET:	cout << "? "; cin >> r[get<1>(program[lr])]; i+=100; lr++; break;
        case PUT:	cout << "> " << r[get<1>(program[lr])] << endl; i+=100; lr++; break;

        case LOAD:	r[get<1>(program[lr])] = pam[r[0]]; i+=10; lr++; break;
        case STORE:	pam[r[0]] = r[get<1>(program[lr])]; i+=10; lr++; break;

        case ADD:   r[get<1>(program[lr])] += pam[r[0]] ; i+=10; lr++; break;
        case SUB:   if( r[get<1>(program[lr])] >= pam[r[0]] )
                        r[get<1>(program[lr])] -= pam[r[0]];
                    else
                        r[get<1>(program[lr])] = 0;
                    i+=10; lr++; break;
        case COPY:	r[0] = r[get<1>(program[lr])] ; i+=1; lr++; break;
        case SHR:   r[get<1>(program[lr])] >>= 1; i+=1; lr++; break;
        case SHL:   r[get<1>(program[lr])] <<= 1; i+=1; lr++; break;
        case INC:   r[get<1>(program[lr])]++ ; i+=1; lr++; break;
        case DEC:   if( r[get<1>(program[lr])]>0 ) r[get<1>(program[lr])]--; i+=1; lr++; break;
        case ZERO: r[get<1>(program[lr])] = 0; i+=1; lr++; break;

        case JUMP: 	lr = get<2>(program[lr]); i+=1; break;
        case JZERO:	if( r[get<1>(program[lr])]==0 ) lr = get<2>(program[lr]); else lr++; i+=1; break;
        case JODD:	if( r[get<1>(program[lr])] % 2 != 0 ) lr = get<2>(program[lr]); else lr++; i+=1; break;
        default: break;
        }
        if( lr<0 || lr>=(int)program.size() )
        {
            cout << "Blad: Wywolanie nieistniejacej instrukcji nr " << lr << "." << endl;
            return -1;
        }
    }
    info << "Skończono program (czas: " << i << ")." << endl;
    return 0;
}
