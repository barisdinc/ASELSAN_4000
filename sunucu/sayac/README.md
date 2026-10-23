# Sayaç sunucusu (Cloudflare Worker)

Programlama sayfasının (`disasm/sim/yukle.html`) düğmelerinin yanındaki sayaçları ve sayfanın altındaki **Bağlantı kaydı** listesini tutar:

- **Sayaçlar:** EEPROM yedeği, flash yedeği, EEPROM yazma (3. adımda Telsize yaz), firmware yükleme (5. adım).
- **Kayıt:** Telsize bağlanma ve bu dört işlem için zaman, maskeli IP, telsizin yazılım sürümü. En son 300 satır tutulur.

Yalnız gerçek telsizle (Web Serial) yapılan ve **başarıyla biten** işlemler sayılır; simülatör modu sayılmaz.

## Gizlilik

IP adresi isteğin kendisinden (`CF-Connecting-IP`) alınır ve yalnız maskeli hâli saklanır: `85.105.12.x`, IPv6'da ilk üç grup (`2a02:4e0:2d2a:x`). Tam adres ne depoda ne yanıtta bulunur. Aynı adresten aynı işlem 5 saniye içinde tekrar gelirse sayılmaz.

## Kurulum (bir kez)

Node.js gerekir (`npx` ile `wrangler` kendisi iner).

```sh
cd sunucu/sayac
npx wrangler login                       # tarayıcıda Cloudflare hesabına izin
npx wrangler kv namespace create SAYAC   # çıkan id'yi wrangler.toml'daki BURAYA_KV_ID yerine yazın
npx wrangler deploy                      # adres: https://aselsan-sayac.<hesap>.workers.dev
```

Sonra `disasm/sim/yukle.html`'deki `SAYAC_VARSAYILAN` sabitine bu adresi yazın (ya da adresi bana verin). Denemek için sayfayı `yukle.html?sayac=https://aselsan-sayac.<hesap>.workers.dev` ile açın; bu parametre simülatör işlemlerini de sayar.

## API

| İstek | Yanıt |
|---|---|
| `GET /durum?n=100` | `{ sayac: {ee_yedek, flash_yedek, ee_yaz, flash_yaz}, kayit: [{t, ip, tur, ek?}] }` |
| `POST /olay` `{tur, ek?}` | `{ sayac, sayildi }`; `tur`: `ee_yedek` `flash_yedek` `ee_yaz` `flash_yaz` `baglanti` |

`POST` yalnız `https://barisdinc.github.io`, `localhost` ve `file://` (origin `null`) sayfalarından kabul edilir. Bu, tarayıcı dışından sahte istek gönderilmesini tam engellemez; sayılar kötü niyetle şişirilebilir.

KV "son yazan kazanır" bir depo: aynı anda gelen iki işlemden biri kaybolabilir. Bu sayfanın trafiğinde önemsiz. Ücretsiz planda günde 1000 KV yazma sınırı var; her sayılan işlem bir yazmadır.

## Test

```sh
make -C disasm test-sayac      # jsc -m test.mjs (ya da node test.mjs)
```
