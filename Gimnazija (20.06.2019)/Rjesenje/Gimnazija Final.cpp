#include<iostream>
#include<string>
#include<exception>
#include<regex>
#include<fstream>
#include<sstream>
#include<vector>
#include<thread>
#include<mutex>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";

enum Sortiranje { ASC, DESC };
enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };
string PredmetString[] = { "MATEMATIKA", "FIZIKA", "HEMIJA", "GEOGRAFIJA", "NOT_SET" };
enum Razred { I = 1, II, III, IV };

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template <class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int _trenutnoElemenata;
	bool _dozvoliDupliranje;
	Sortiranje _sortiranje;
public:
	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true) {
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		_dozvoliDupliranje = dozvoliDupliranje;
	}
	Kolekcija(const Kolekcija& org)
	{
		_trenutnoElemenata = org._trenutnoElemenata;
		for (int i = 0; i < _trenutnoElemenata; i++)
		{
			_elementi1[i] = new T1(*org._elementi1[i]);
			_elementi2[i] = new T2(*org._elementi2[i]);
		}
		_dozvoliDupliranje = org._dozvoliDupliranje;
		_sortiranje = org._sortiranje;
	}
	~Kolekcija() {
		for (size_t i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	void AddElement(const T1& el1, const T2& el2)
	{
		if (_trenutnoElemenata == max) throw exception("Dostignut maksimalan broj elemenata!\n");
		if(!_dozvoliDupliranje)
			for (int i = 0; i < _trenutnoElemenata; i++)
				if (*_elementi1[i] == el1 && *_elementi2[i] == el2)
					throw exception("Nedozvoljeno dupliranje elemenata!\n");
		_elementi1[_trenutnoElemenata] = new T1(el1);
		_elementi2[_trenutnoElemenata++] = new T2(el2);
		for (int i = 0; i < _trenutnoElemenata - 1; i++)
		{
			for (int j = 0; j < _trenutnoElemenata - 1 - i; j++)
			{
				if (_sortiranje == ASC && *_elementi1[j] > *_elementi1[j + 1])
				{
					swap(*_elementi1[j], *_elementi1[j + 1]);
					swap(*_elementi2[j], *_elementi2[j + 1]);
				}
				else if (_sortiranje == DESC && *_elementi1[j] < *_elementi1[j + 1])
				{
					swap(*_elementi1[j], *_elementi1[j + 1]);
					swap(*_elementi2[j], *_elementi2[j + 1]);
				}
			}
		}
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class Aktivnost {
	shared_ptr<Razred> _razred;
	string _opis;
	int _ocjena;//1-5
public:
	Aktivnost(Razred razred = I, int ocjena = 0, string opis = not_set) {
		_ocjena = ocjena;
		_opis = opis;
		_razred = make_shared<Razred>(razred);
	}
	int  GetOcjenu() const { return _ocjena; }
	string GetOpis() const { return _opis; }
	Razred GetRazred() const { return *_razred; }
	friend bool operator==(const Aktivnost& p, const Aktivnost& d)
	{
		return *p._razred == *d._razred && p._opis == d._opis && p._ocjena == d._ocjena;
	}
	friend ostream& operator<<(ostream& COUT, const Aktivnost& obj) {
		COUT << "Razred: " << *obj._razred << " Ocjena: " << obj._ocjena << " Opis: " << obj._opis << endl;
		return COUT;
	}
};

class Polaznik {
protected:
	char* _imePrezime;
	string _brojTelefona;
public:
	Polaznik(string imePrezime, string brojTelefona) : _imePrezime(AlocirajNizKaraktera(imePrezime.c_str())) {
		_brojTelefona = brojTelefona;
	}
	Polaznik(const Polaznik& org)
	{
		_imePrezime = AlocirajNizKaraktera(org._imePrezime);
		_brojTelefona = org._brojTelefona;
	}
	~Polaznik() { delete[] _imePrezime; }
	Polaznik&operator=(const Polaznik& org)
	{
		if (this != &org)
		{
			_imePrezime = AlocirajNizKaraktera(org._imePrezime);
			_brojTelefona = org._brojTelefona;
		}
		return *this;
	}
	char* GetImePrezime() { return _imePrezime; }
	string GetTelefon() { return _imePrezime; }
	virtual void PredstaviSe() = 0;
	friend bool operator==(const Polaznik& p, const Polaznik& d)
	{
		return strcmp(p._imePrezime, d._imePrezime) == 0 && p._brojTelefona == d._brojTelefona;
	}
};

class Ucenik : public Polaznik{
	Kolekcija<Predmet, Aktivnost, 16>* _aktivnosti;
public:
	Ucenik(const char* imePrezime, string brojTelefona) : Polaznik(imePrezime, brojTelefona) { _aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>; }
	Ucenik(const Ucenik& org) : Polaznik(org) { _aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>(*org._aktivnosti); }
	~Ucenik() { delete _aktivnosti; _aktivnosti = nullptr; }
	Kolekcija<Predmet, Aktivnost, 16>& GetAktivnosti() { return *_aktivnosti; };
	virtual void PredstaviSe(){}
	friend ostream& operator<<(ostream& COUT, Ucenik& n)
	{
		for (int i = 0; i < n.GetAktivnosti().GetTrenutno(); i++)
		{
			COUT << "Predmet: " << PredmetString[n.GetAktivnosti().GetElement1(i)] << endl;
			COUT << "Aktivnost: " << n.GetAktivnosti().GetElement2(i) << endl;
		}
		return COUT;
	}
	float getProsjek() {
		float prosjek = 0;
		for (int i = 0; i < _aktivnosti->GetTrenutno(); i++)
			prosjek += _aktivnosti->GetElement2(i).GetOcjenu();
		return prosjek / _aktivnosti->GetTrenutno();
	}

};
mutex m;
class Skola {
	char* _naziv;
	vector<Ucenik> _ucenici;
public:
	Skola(const char* naziv = nullptr) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Skola() {
		delete[] _naziv; _naziv = nullptr;
	}
	char* GetNaziv()const { return _naziv; }
	vector<Ucenik>& GetUcenici() { return _ucenici; };
	void operator()(const char* imePrezime, string brojTelefona)
	{
		for (auto ucenik : _ucenici)
		{
			if (strcmp(ucenik.GetImePrezime(), imePrezime) == 0 || ucenik.GetTelefon() == brojTelefona)
				throw exception("Nedozvoljeno dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona!");
		}
		if(!regex_match(brojTelefona,regex("(\\+387\\(6\\d\\)\\d{3}-\\d{3})|(\\+387\\s6\\d\\s\\d{3}-\\d{3})")))
			brojTelefona=not_set;
		_ucenici.push_back(Ucenik(imePrezime, brojTelefona));
	}
	void ispis(Aktivnost aktivnost, Ucenik ucenik) {
		m.lock();
		cout << "Uspjesno ste okoncali aktivnosti u okviru " << aktivnost.GetRazred() << " razreda sa prosjecnom ocjenom " << ucenik.getProsjek() << endl;
		m.unlock();
	}
	bool DodajAktivnost(const char* imePrezime, Predmet predmet, Aktivnost aktivnost)
	{
		int brojac = 0;
		for (auto& ucenik : _ucenici)
		{
			for (int i = 0; i < ucenik.GetAktivnosti().GetTrenutno(); i++)
			{
				if (ucenik.GetAktivnosti().GetElement2(i).GetRazred() == aktivnost.GetRazred())
					brojac++;
				if (ucenik.GetAktivnosti().GetElement2(i) == aktivnost)
					return false;
			}
		}
		if (brojac >= 4) return false;
		for (auto& ucenik : _ucenici)
		{
			if (strcmp(ucenik.GetImePrezime(), imePrezime) == 0)
			{
				ucenik.GetAktivnosti().AddElement(predmet, aktivnost);
				if (aktivnost.GetOcjenu() > 1)
				{
					thread t1(&Skola::ispis, this, aktivnost, ucenik);
					t1.join();
				}
				return true;
			}
		}
		return false;
	}
	pair<Polaznik*, float> GetNajboljegUcenika()
	{
		int index = 0;
		float najprosjek = 0;
		for (int i=0;i<_ucenici.size();i++)
		{
			if (_ucenici[i].getProsjek() > najprosjek)
			{
				najprosjek = _ucenici[i].getProsjek();
				index = i;
			}
		}
		return { &_ucenici[index],najprosjek };
	}
	///*U fajl (npr. Gimnazija.txt) upisati podatke (podatke upisati kao obicni tekst) o skoli i svim ucenicima.
	//Nakon upisa, potrebno je ispisati sadrzaj fajla. Parametar tipa bool oznacava da li ce ranije dodani sadrzaj fajla prethodno biti pobrisan*/

	//if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt"))
	//	cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;
	//if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt", true))
	//	cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;
	bool SpasiUFajl(string fajl, bool brisati=false)
	{
		ofstream upis;
		if (!brisati)
			upis.open(fajl, ios::app);
		else
			upis.open(fajl);
		if (upis.fail())
			return false;
		upis << *this;
		ifstream ispis(fajl);
		if (ispis.fail())
			return false;
		char znak;
		while (ispis.get(znak))
		{
			cout << znak;
		}
		return true;
	}
	friend ostream& operator<<(ostream& COUT, Skola& obj) {
		COUT << "Skola: " << obj._naziv << endl;
		COUT << "Ucenici: " << endl;
		for (size_t i = 0; i < obj._ucenici.size(); i++)
			COUT << obj._ucenici[i] << endl;
		return COUT;
	}
};

int main() {
	Kolekcija<int, int, 10> kolekcija1(DESC, false);
	try {
		kolekcija1.AddElement(1, 2);
		//dupliranje elemenata nije dozvoljeno
		kolekcija1.AddElement(1, 2);
	}
	catch (exception & ex) {
		cout << ex.what();
	}

	/*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/
	for (size_t i = 1; i < kolekcija1.GetMax() - 1; i++)
		kolekcija1.AddElement(rand(), rand());

	cout << kolekcija1 << endl;

	try {
		//prekoracen maksimalan broj elemenata
		kolekcija1.AddElement(rand(), rand());
	}
	catch (exception & ex) {
		cout << ex.what();
	}
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;

	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");
	//dodaje novog ucenika u skolu
	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");
	gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/
	gimnazijaMostar("Telefon NotValidFormat", "387 61)333-444");

	try
	{
		/*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/
		gimnazijaMostar("Adel Handzic", "+387(61)333-444");
	}
	catch (exception & ex)
	{
		cout << ex.what() << endl;
	}

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti, a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))
		cout << "Aktivnost nije uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda, te posjeduje validan broj telefona,
	u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa prosjecnom ocjenom X.X"*/
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))
		cout << "Aktivnost uspjesno dodana" << endl;

	//ispisuje sve ucenike i njihove aktivnosti
	cout << gimnazijaMostar << endl;


	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();
	cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;

	/*U fajl (npr. Gimnazija.txt) upisati podatke (podatke upisati kao obicni tekst) o skoli i svim ucenicima.
	Nakon upisa, potrebno je ispisati sadrzaj fajla. Parametar tipa bool oznacava da li ce ranije dodani sadrzaj fajla prethodno biti pobrisan*/

	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt"))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;
	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt", true))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;

	return 0;
}

