#include<iostream>
#include<regex>
#include<mutex>
#include<string>
#include<vector>
using namespace std;
const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO RE\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE!\n"
"7. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}
template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Kolekcija(bool omoguciDupliranje = true) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
		_omoguciDupliranje = omoguciDupliranje;
	}
	Kolekcija(const Kolekcija& org) {
		_trenutno = org._trenutno;
		_omoguciDupliranje = org._omoguciDupliranje;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (int i = 0; i < _trenutno; i++) {
			_elementi1[i] = org._elementi1[i];
			_elementi2[i] = org._elementi2[i];
		}
	}
	friend bool operator==(const Kolekcija& prvi, const Kolekcija& drugi) {
		if (prvi._trenutno != drugi._trenutno) return false;
		for (int i = 0; i < prvi._trenutno; i++) {
			if (!(prvi._elementi1[i] == drugi._elementi1[i] && prvi._elementi2[i] == drugi._elementi2[i]))
				return false;
		}
		return true;
	}
	Kolekcija& operator=(const Kolekcija& org) {
		if (this != &org) {
			_trenutno = org._trenutno;
			_omoguciDupliranje = org._omoguciDupliranje;
			delete[] _elementi1;
			delete[] _elementi2;
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (int i = 0; i < _trenutno; i++) {
				_elementi1[i] = org._elementi1[i];
				_elementi2[i] = org._elementi2[i];
			}
		}
		return *this;
	}
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
	}
	void AddElement(const T1& el1, const T2& el2) {
		if (_omoguciDupliranje == false) {
			for (int i = 0; i < _trenutno; i++) {
				if (_elementi1[i] == el1 || _elementi2[i] == el2) throw exception("Nije dozvoljeno dupliranje");
			}
		}
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno++] = el2;
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	Kolekcija<T1, T2> operator()(int min, int max) {
		Kolekcija<int, int> nova;
		if (min < 0 || min >= _trenutno || max < 0 || max >= _trenutno || max < min) throw exception("Nedozvoljene lokacije");
		for (int i = min; i <= max; i++) {
			nova.AddElement(_elementi1[i], _elementi2[i]);
		}
		return nova;
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& org) {
		_dan = new int(*org._dan);
		_mjesec = new int(*org._mjesec);
		_godina = new int(*org._godina);
	}
	Datum& operator=(const Datum& org) {
		if (this != &org) {
			*_dan = *org._dan;
			*_mjesec = *org._mjesec;
			*_godina = *org._godina;
		}
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	int brojPrestupnihGodina(int g) const
	{
		if (*_mjesec == 1 || *_mjesec == 2 && *_dan < 29)
			g--;
		return g / 4 + g / 400 - g / 100;
	}
	int getDani() const
	{
		int ukupno = *_dan + *_godina * 365;
		int mjeseci[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		for (int i = 0; i < *_mjesec - 1; i++)
			ukupno += mjeseci[i];
		ukupno += brojPrestupnihGodina(*_godina);
		return ukupno;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	friend int operator -(const Datum& p, const Datum&d)
	{
		return p.getDani() - d.getDani();
	}
	friend bool operator >=(const Datum& p, const Datum&d)
	{
		return p.getDani() >= d.getDani();
	}
	friend bool operator==(const Datum& p,const Datum&d) {
		return *p._dan == *d._dan && *p._mjesec == *d._mjesec && *p._godina == *d._godina;
	}
};

class Predmet {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
	Kolekcija<int, Datum> _ocjene;
public:
	Predmet(const char* naziv = "", int ocjena = 0, Datum datum = Datum()) {
		_naziv = GetNizKaraktera(naziv);
		if (ocjena > 0)
			_ocjene.AddElement(ocjena, datum);
	}
	Predmet(const Predmet& org) {
		_naziv = GetNizKaraktera(org._naziv);
		_ocjene = org._ocjene;
	}
	Predmet& operator=(const Predmet& org) {
		if (this != &org) {
			delete[] _naziv;
			_naziv = GetNizKaraktera(org._naziv);
			_ocjene = org._ocjene;
		}
		return *this;
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	void AddOcjena(int ocjena, Datum datum) {
		_ocjene.AddElement(ocjena, datum);
	}
	float getProsjekDatum() {
		float prosjek = 0;
		for (int i = 0; i < _ocjene.getTrenutno() - 1; i++)
		{
			prosjek += abs(_ocjene.getElement2(i + 1) - _ocjene.getElement2(i));
		}
		return prosjek / _ocjene.getTrenutno();
	}
	char* GetNaziv() { return _naziv; }
	Kolekcija<int, Datum>& GetOcjene() { return _ocjene; }
	float GetProsjek() {
		float prosjek = 0;
		if (_ocjene.getTrenutno() == 0) return 0;
		for (int i = 0; i < _ocjene.getTrenutno(); i++) {
			prosjek += _ocjene.getElement1(i);
		}
		return prosjek / _ocjene.getTrenutno();
	}
	bool operator==(const Predmet& drugi) {
		return _ocjene == drugi._ocjene && strcmp(_naziv, drugi._naziv) == 0;
	}
	friend ostream& operator<<(ostream& COUT, Predmet& obj) {
		COUT << obj._naziv << endl;
		for (int i = 0; i < obj._ocjene.getTrenutno(); i++)
			cout << "Ocjena: " << obj._ocjene.getElement1(i) << ", Datum: " << obj._ocjene.getElement2(i) << endl;
		COUT << "Prosjecna: " << obj.GetProsjek() << endl;
		return COUT;
	}
};
class Uspjeh {
	eRazred _razred;
	//string se odnosi na napomenu o polozenom predmetu
	Kolekcija<Predmet, string>* _polozeniPredmeti;
public:
	Uspjeh(eRazred razred = PRVI) { // na ovom ispitu _polozeniPredmeti nije bilo postavljeno u konstruktoru, i tvoj izbor je da ili alociras memoriju odmah
		_razred = razred;			// ili da stavis kolekciju na nullptr, ja sam stavio nullptr kao izazov u upravljanju memorijom 
		_polozeniPredmeti = nullptr;// Na ispitu nekad dodje pokazivac vec stavljen na nullptr i tada morate znati upravljati memorijom (tj. alocirati prije nego sto pristupate kolekciji
	}								// da ne bi se desio runtime error (najcesca pogreska kod vecine ljudi), a u tome da ne dirate konstruktor kakav je profesor zadao

	~Uspjeh() { delete _polozeniPredmeti; _polozeniPredmeti = nullptr; }
	Uspjeh(const Uspjeh& org) {
		_razred = org._razred;
		if (org._polozeniPredmeti != nullptr) {
			_polozeniPredmeti = new Kolekcija<Predmet, string>(*org._polozeniPredmeti);
		}
		else
			_polozeniPredmeti = nullptr;
	}
	void Alociraj() { _polozeniPredmeti = new Kolekcija <Predmet, string>; }
	Kolekcija<Predmet, string>* GetPredmeti() { return _polozeniPredmeti; }
	eRazred GetERazred() { return _razred; }
	friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
		COUT << "Razred: " << obj._razred << endl;
		for (int i = 0; i < obj._polozeniPredmeti->getTrenutno(); i++)
		{
			cout << "Predmet: " << obj._polozeniPredmeti->getElement1(i) << "Napomena: " << obj._polozeniPredmeti->getElement2(i) << endl << endl;
		}
		return COUT;
	}
	float prosjekRazred() {
		float prosjek = 0;
		if (_polozeniPredmeti != nullptr) {
			for (int i = 0; i < _polozeniPredmeti->getTrenutno(); i++) {
				prosjek += _polozeniPredmeti->getElement1(i).GetProsjek();
			}
		}
		return prosjek / _polozeniPredmeti->getTrenutno();
	}

};
mutex m;
bool ValidirajEmail(string mail) {
	string pravilo = "(\\w+\\.?\\w+)@(fit|edu.fit)(.ba|.com|.org)";
	if (regex_match(mail, regex(pravilo))) return true;
	return false;
}
class Kandidat {
	char* _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;
public:
	Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = GetNizKaraktera(imePrezime);
		if (ValidirajEmail(emailAdresa))
			_emailAdresa = emailAdresa;
		else
			_emailAdresa = "notSet@edu.fit.ba";
		_brojTelefona = brojTelefona;
	}
	~Kandidat() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		for (size_t i = 0; i < obj._uspjeh.size(); i++)
			COUT << obj._uspjeh[i] << endl;
		return COUT;
	}
	float prosjekSkolovanja() {
		float prosjek = 0;
		for (int i = 0; i < _uspjeh.size(); i++) {
			prosjek += _uspjeh[i].prosjekRazred();
		}
		return prosjek / _uspjeh.size();
	}
	vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char* GetImePrezime() { return _imePrezime; }
	void PosaljiSMS(eRazred r, float prosjekRazred) {
		m.lock();
		cout << "Svaka cast za uspjeh " << prosjekRazred << " u " << r << " razredu" << endl;
		m.unlock();
	}
	void PosaljiEmail(eRazred r, float prosjekRazred) {
		m.lock();
		cout << "FROM:info@fit.ba" << endl;
		cout << "TO:" << _emailAdresa << endl;
		cout << "Postovani " << _imePrezime << " evidentirali ste uspjeh za " << r << " razred. Dosadasnji uspjeh (prosjek) na nivou "
			<< r << ". razreda iznosi " << prosjekRazred << ", a ukupni uspjeh u toku skolovanja iznosi " << prosjekSkolovanja() << endl;
		cout << "Pozdrav.\n FIT Team" << endl;
		m.unlock();
	}
	bool AddPredmet(eRazred razred, Predmet p, string napomena) {
		if (p.GetProsjek() < 2.5) return false;
		for (int i = 0; i < _uspjeh.size(); i++) {
			if (_uspjeh[i].GetERazred() == razred) {
				if (_uspjeh[i].GetPredmeti()->getTrenutno() == 5) return false;

				for (int j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
					if (_uspjeh[i].GetPredmeti()->getElement1(j) == p) return false;
				
				_uspjeh[i].GetPredmeti()->AddElement(p, napomena);
				thread t1(&Kandidat::PosaljiEmail, this, razred, _uspjeh[i].prosjekRazred());
				t1.join();
				if (_uspjeh[i].prosjekRazred() > 4.5)
				{
					thread t2(&Kandidat::PosaljiSMS, this, razred, _uspjeh[i].prosjekRazred());
					t2.join();
				}
				return true;
			}
		}
		Uspjeh u(razred);
		u.Alociraj();
		u.GetPredmeti()->AddElement(p, napomena);
		_uspjeh.push_back(u);
		thread t2(&Kandidat::PosaljiEmail, this, razred, u.prosjekRazred());
		t2.join();
		if (u.prosjekRazred() > 4.5)
		{
			thread t3(&Kandidat::PosaljiSMS, this, razred, u.prosjekRazred());
			t3.join();
		}
		return true;
	}
	Kolekcija<Predmet, float> operator()(Datum pocetak, Datum kraj)
	{
		Kolekcija<Predmet, float> temp;
		for (auto uspjeh : _uspjeh)
		{
			for (int i = 0; i < uspjeh.GetPredmeti()->getTrenutno(); i++)
			{
				for (int j = 0; j < uspjeh.GetPredmeti()->getElement1(i).GetOcjene().getTrenutno(); j++)
				{
					Datum datum = uspjeh.GetPredmeti()->getElement1(i).GetOcjene().getElement2(j);
					if (datum >= pocetak && kraj >= datum)
					{
						temp.AddElement(uspjeh.GetPredmeti()->getElement1(i), uspjeh.GetPredmeti()->getElement1(i).getProsjekDatum());
						break;
					}
				}
			}
		}
		return temp;
	}
	Uspjeh* operator[] (eRazred razred)
	{
		for (auto& uspjeh : _uspjeh)
			if (uspjeh.GetERazred() == razred) // Ako nadje taj uspjeh, vrati njegovu adresu jer je operator[] pokazivac na uspjeh
				return &uspjeh;
		return nullptr;
	}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Na sta se odnosi pojam reprezentacije tipa?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Na koji nacin se moze izbjeci pojavljivanje vise podobjekata bazne klase u slucaju visestrukog nasljedjivanja?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}


int main() {

	cout << PORUKA;
	//cin.get();

	cout << GetOdgovorNaPrvoPitanje() << endl;
	//cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	//cin.get();

	Datum temp,
		datum19062019(19, 6, 2019),
		datum20062019(20, 6, 2019),
		datum30062019(30, 6, 2019),
		datum05072019(5, 7, 2019);

	int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(false);
	for (int i = 0; i <= kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
		kolekcija1.AddElement(3, 3);
	}
	catch (exception & err) {
		cout << err.what() << crt;
	}
	cout << kolekcija1 << crt;

	/*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
	ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
	Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
	cout << kolekcija2 << crt;
	try {
		//primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
		Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
		Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
	}
	catch (exception & err) {
		cout << err.what() << crt;
	}

	//parametri: nazivPredmeta, prva ocjena, datum
	Predmet Matematika("Matematika", 5, datum19062019),
		Fizika("Fizika", 5, datum20062019),
		Hemija("Hemija", 2, datum30062019),
		Engleski("Engleski", 5, datum05072019);

	Matematika.AddOcjena(3, datum05072019);
	Matematika.AddOcjena(5, datum05072019);

	// ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
	// ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
	cout << Matematika << endl;

	if (ValidirajEmail("text.text@edu.fit.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@edu.fit.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@fit.ba"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@fit.com"))
		cout << "Email validan" << crt;
	if (ValidirajEmail("texttext@edu.fit.org"))
		cout << "Email validan" << crt;

	///*
	//email adresa mora biti u formatu text.text (sa ili bez tacke),
	//dok domena moze biti fit.ba ili edu.fit.ba
	//nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.

	//za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
	//validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
	//postaviti je na defaultnu adresu: notSet@edu.fit.ba
	//*/

	Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
	Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

	///*
	//uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	//tom prilikom onemoguciti:
	//- dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda, ... pitajte na ispitu sta znaci to u zagradi, tj. sta se mora tacno poredit
	//- dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
	//- dodavanje vise od 5 predmeta na nivou jednog razreda
	//razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
	//Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	//*/
	if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
		cout << "Predmet uspjesno dodan!" << crt;

	///*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
	//FROM:info@fit.ba
	//TO: emailKorisnika
	//Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
	//na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
	//Pozdrav.
	//FIT Team.
	//ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
	//slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	//*/
	cout << jasmin << crt;

	//vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
	//float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
	Kolekcija<Predmet, float> jasminUspjeh = jasmin(Datum(18, 06, 2019), Datum(21, 06, 2019));
	cout << jasminUspjeh << crt;
	Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
	if (uspjeh_Irazred != nullptr)
		cout << *uspjeh_Irazred << crt;

	//cin.get();
	//system("pause>0");
	return 0;
}
