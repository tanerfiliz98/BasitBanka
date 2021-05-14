# Basit Banka
 C dilinde konsolda basit bir banka uygulaması.Uygulamada giriş ekranında müşteri numarası ve pin istenir. Kullanıcı ekleme, Hesap ekleme ve silme işlemleri bankanın özel Hesabı ile yapılabilir. Uygulamada bir kişinin birden fazla hesabı olabilir. Bankada Ticari ve Bireysel olmak üzere iki kullanıcı türü vardır. Kullanıcı tipleri müşteri numarıların baş numarasıyla algılanır.  Kullanıcılar Para çekme, Para yatırma, Başka hesaba havale işlemlerini yapabilir. Para çekme kısmında kullanıcılar tiplerine göre günlük limit dahilinde para çekebilirler (Ticari 1500,Bireysel 750). Her bir kullanıcı için ek bir hesap oluşturulmuştur (Ticari 1000, Bireysel 500) ve ek hesaptan işlem yapıldığı zaman kullanıcının herhangi bir hesabına para geldiği zaman ek hesaptan para düşürülür. Bankanın özel hesabı ile yeni müşteri ekleme, Kayıtlı müşteriye hesap açma, Müşterinin hesabını silme ve bankadaki toplam paraya bakma işlemlerini yapabilir.
 
 
 
 ## Projenin Çalısması için Gerekeneler
 - C kodlarını derleyebilen bir derleyici

## Proje Nasıl Çalıştırılır ve Dosya Yapısı

Projeyi çalıştırmak için özel birşey gerekmemektedir.
Bankanın özel hesabının müşteri numarası: ```654321``` şifresi ```1234```
#### ticari.txt ve bireysel.txt dosyalarında
- her bir kullanıcının başında  ```*``` işareti vardır
- İlk önce müşteri numarası 
- hesap sayısı
- pini
- Kullanıcın adı (boşluk karakteri için ```.``` kullanılmıştır)
- Günlük Para çekme limiti ve o günün tarihi (yıl/ay/gün)
- ```_``` işareti kullanıcının hesaplarının içine girildiği söylenir
- hesap numaraları
Bunlar her kullanıcı için tekrarlanır.

#### Hesaplar klasorünün içerisindekiler
- {MüşteriNumarası}.txt olan dosyada kullanıcıların ek hesaplarının bakiyesi tutulur
- {MüşteriNumarası}{HesapNumarası}.txt adlı dosyada
  - hesaptaki bakiyesi
  - hesapta gerçekleştirelen para işlemlerinin tarihi ve gerçekleştirilen olay
