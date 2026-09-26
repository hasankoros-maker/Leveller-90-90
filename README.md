# Leveller Geode Mod

Geometry Dash **2.2.144** için Geode mod başlangıcı. Level sayfasında normal oynatma düğmesinin yanında ayrı bir **BOT** düğmesi gösterir.

## Bot prototipi

`BOT` düğmesi yalnızca seçilmiş zor seviyelerde etkinleşir:

- Detah Corridor / Death Corridor
- Ton 618

Bot, level nesnelerini oyun içinden tarayan **yerel ve deneysel bir heuristik analiz** kullanır. Yakındaki tehlike ve katı engellere göre tek karelik zıplama girişleri üretir. Bu, evrensel bir çözücü değildir; tüm seviyeleri bitirme garantisi vermez ve özellikle gemi, wave, dual, portal zincirleri ve hassas orb sekanslarında başarısız olabilir.

Bot yalnızca normal modda çalışır; practice mode'da ve seçili liste dışındaki seviyelerde devre dışıdır. `Analysis strength` ayarı ileriye dönük tarama mesafesini 1-5 arasında değiştirir.

## Derleme

Geode SDK 2.0.0-beta.27 ve Geometry Dash 2.2.144 toolchain'i ile:

```sh
geode build
geode build -p android64
```

CI, Android64 paketini üretmek üzere yapılandırılmıştır.

## Uyumluluk ve yayın notu

Manifest Windows, Android ve macOS için **2.2.144** hedefler. Android cihazda arm64-v8a gerekir. Geode indeksine gönderilmeden önce gerçek oyunda test edilmelidir; resmi indeks başvurusu yönetici onayına tabidir.
