# ASELSAN_4000
ASELSAN 4000 Serisi Telsizlerin Guncel Ozelliklere Kavusturulmasi
-----------------------------------------------------------------
Bu proje kapsaminda ASELSAN 4000 serisi VHF ve UHF telsizlerinin modern VHF/UHF telsizlerle benzer ozelliklere kavusturulmasi amaclanmaktaidir. Projenin ilk fikirleri 2019 senesi ortalarinda olusturulmustur.

Telsiz cihazinda kullanilan Mitsubishi firmasina ait islemcinin Mitsubishi tarafindan uretimi ve destegi 90'li yillarda sonlandirilmistir. Daha sonra mitsubishi firmasi Renesas firmasi tarafindan satin alinmistir. Bu nedenle teslsiz uzerindeki islemci ile ilgili olarak hicbir kaynaga erisim mumkun olmamistir. Konu ile ilgili olarak ASELSAN'daki mevcut kaynaklara ulasilmaya calisilmis, RENESAS firmasina ulasilmaya calisilmis ya da internette kullanilabilecek islemci araclari (derleyici, debugger, dokuman, vb) taranmis ancak basarisiz olunmustur.

Projeye ilk basladigimizda sadece yeni bir firmware yaziop mevcut cihaza sadece firmware guncellemesi yaparak cozum olusturulmasi hedeflenmisti, ancak islemci verilerine ulasamadigimiz icin alternatiflari arastirmaya basladik.
Islemcinin degistirilmesine karar verildi. SMD ve cok bacakli olan islemci ile bacak uyumu olan baska bir islemci arandi ancak bulunamadi (dusuk bir ihtimaldi ama denendi).

Yapilacak islem hem herkesin yapabilecegi kadar basit olmaliydi hem de cok ugrastirmamaliydi. TRAC Golcuk Subesi Baskani UMUT OCAL'in (TA2AUP) hibe ettigi bir telsiz uzerinde fiziksel incelemeler yaparak basladik.  Ilk denemelerini Cem Karagoz'un (TA2GY) yaptigi eski islemcinin yerine lehimlenecek bir tasiyici kart ve uzerinde yeni islemci oldukca umut verici oldu.
Islemci olarak MSP430 kullanmaya karar verdik simdilik. Aslinda yeni nesil islemcilerin kullanilmasi, hatta direk uzerinde bluetooth, WIFI olan birseyler kullanilmasi eminim ki cok daha akillica olurdu, ozellikle fiyatlarin bu denli yerlerde surundugu bir donemde. Ancak elimizde hurdadan alidigmiz 1000'den (bin) fazla MSP430 vardi ve bir yandan da bunlari da telsizler gibi yeniden hayata dondurmek istiyorduk.. Birsykilde hepsi milli servet.


Kolari sivadik ve 2020 senesinin pandemi gunlerinde bu calismaya basladik
Baris DINC
TA7W / OH2UDS
Haziran 2020
