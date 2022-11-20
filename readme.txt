Kodun amacı:

Server process: Tek bir process olarak çalışır. İstemcilerle (client) message passing yöntemi ile veri
paylaşır. Multithreaded olarak çalışır: main thread ve her bir istemci için birer worker thread. Main
thread belirli bir maibox ı dinler ve görevi kimliği verilen istemciyi kaydetmek ve onun adına bir worker
thread oluşturmaktır. İstemci daha sonra tüm haberleşmesini bu worker thread üzerinden yapar.
Worker thread kendisine gelen bir mesajı kimliği verilen istemciye gönderir.

Client process: Process komut satırından çalıştırılırken argv[1] olarak istemcinin mesajlaşmak için
kullanacağı kimlik verlilir. İstemci döngü içinde kime mesaj göndereceğini ve mesaj içeriğini (string)
kullanıcıdan alır ve sunucuya gönderir. Kendine gelen mesajları ise standart çıktıya yazdırır.

Compile etmek için
	rm  -f server
	gcc  -Wall  -g server.c  -o server  -pthread
	gcc  -Wall  -g client.c  -o client  -pthread
yazıyoruz.
çalıştırmak için
    ./server
    ./client emre
    ./client osmaniye80
    ./client 8042
gibi çalıştırıyoruz (daha fazla da client açılabilir).
sonra clientların içinde isimleri criptleyip long yapıyor kod. Sonra da server a yollayıp server da burada alıcının id sinin portuna yolluyor.
Burada da dinleme işlemini yapıyoruz.
içinde istenildiği gibi her client için server da thread oluşturuyo ve worker thread o client la iletişim kuruyor.Main thread ise yeni client geldi mi gelmedi mi diye kontrol ediyor.Gelirse yeni worker thread oluşturuyor. 
clientda da thread main olan mesaj gönderirken worker olan ise server ı dinliyor.Bu sayede girdi beklerken aynı zamanda dinleme işlevi de yapılabiliyor. 

ÇALIŞTIRMA:
ilk önce ./ ların hepsini yazdıktan sonra karşımıza
"Write receiver name:" yazısı çıkıyor burada alıcının key ini girmeniz lazım. Sonra
"Write text : " yazısı çıkıyor burada alıcının mesajını ini girmeniz lazım.

sonuç:
server da gönderen kişinin message type ı ve mesajı yazar alıcı da ise "Text: + mesaj neyse o " yazar.
Ctrl + c ile de client ve serverlar sonlandırılır.
*******************************************************************
Örnek: NOT!!!("Write text" burada iletilmesini istediğin mesajlar "Text" ise sana gelen mesajlar. "^C" ise  Ctrl + c yi ifade eder.)
 
rm  -f server
gcc  -Wall  -g server.c  -o server  -pthread
gcc  -Wall  -g client.c  -o client  -pthread
./server
./client Osman80
./client emre

"emre nin mesajlar"
---------------------
Write receiver name: Osman80

Write text : Selam nasılsın?

Write receiver name: Osman80

Write text : Bil461 ödevini yaptın mı?

Write receiver name:     
Text: ödevi yaptım. 
Osman80

Write text : Uzak etü ye yüklemeyi unutma

Write receiver name: 
Text: Unutmam sağol. 
^C

"Osman80 nin mesajlar"
---------------------

Write receiver name: 
Text: Selam nasılsın? 

Text: Bil461 ödevini yaptın mı? 
emre

Write text : ödevi yaptım.

Write receiver name: 
Text: Uzak etü ye yüklemeyi unutma 
emre 

Write text : Unutmam sağol.
^C

"server ın mesajlar"
---------------------
Message port is 586 Message is: Selam nasılsın?
Message port is 586 Message is: Bil461 ödevini yaptın mı?
Message port is 903 Message is: ödevi yaptım.
Message port is 586 Message is: Uzak etü ye yüklemeyi unutma
Message port is 903 Message is: Unutmam sağol.
^C

*******************************************************************
Hepsi Ctrl + c  ile sonlandırıldı.
