# Leveller Geode Mod

Geometry Dash 2.2.x için Geode mod başlangıç projesi.

## Ne yapıyor?

`LevelPage` açıldığında, mevcut oyunun normal oynatma düğmesinin yanında küçük bir **LEVELLER** düğmesi gösterir. Düğmeye basmak Geometry Dash'in kendi `LevelPage::onPlay` akışını çağırır; bu nedenle normal oyun ekranı açılır.

Bu sürüm henüz seviyeyi otomatik bitiren bot değildir. Evrensel otomatik oynama; PlayLayer giriş hook'ları, seviye objelerinin zamanlaması ve her platform/sürüm için ayrı test gerektirir. Sahte ilerleme veya doğrulama atlatma yapılmaz.

## Derleme

Geode CLI ve SDK kurulu bir ortamda:

```sh
cd Leveller
geode build
```

Android 64-bit:

```sh
geode build -p android64
```

Apple/iOS: kaynak kodu platformdan bağımsız tutulmuştur; derleme ancak ilgili Geode/iOS toolchain ve launcher desteği mevcutsa yapılabilir.

## Sürüm hedefi

Manifest minimum Geometry Dash 2.2 ailesini hedefler. 2.2.144 üzerinde test edilmesi önerilir. Her 2.2.x yaması için Geode SDK ve oyun binary uyumluluğu ayrıca kontrol edilmelidir.
