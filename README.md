# Leveller Geode Mod

Geometry Dash **2.2.144 Android64** için Geode mod başlangıcı. Level sayfasında normal oynatma düğmesinin yanında ayrı bir **BOT** düğmesi gösterir.

## Mevcut davranış

- **PLAY** seçili leveli Geometry Dash'in normal oynatma akışıyla açar.
- **BOT** seçili leveli yerel BOT modu işaretiyle normal oynatma akışında açar.
- Mod sahte ilerleme, doğrulama atlatma veya çevrimiçi skor yazmaz.
- Evrensel olarak her leveli kusursuz bitiren bir bot bu sürümde mevcut değildir; böyle bir motor için PlayLayer sürüm hook'ları, obje zamanlaması ve level bazlı test verisi gerekir.

## Derleme

Geode SDK 2.0.0-beta.27 ve Geometry Dash 2.2.144 toolchain'i ile:

```sh
geode build
geode build -p android64
```

## Uyumluluk

Manifest Windows, Android ve macOS için **2.2.144** hedefler. Geode indeksine yayın yapmadan önce her platformda gerçek oyun binary'siyle test edilmelidir.
