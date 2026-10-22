# Kullanım kılavuzu

| Dosya | İçerik |
|---|---|
| `KULLANIM_KILAVUZU_AT_V<sürüm>.pdf` | Yazdırılabilir kılavuz (kapak, içindekiler, her bölüm yeni sayfada) |
| `kullanim_kilavuzu.html` | Aynı kılavuz, tarayıcıda (açık / koyu tema) |
| `kaynak.html` | Elle düzenlenen kaynak. Tuşlar kısaltmayla yazılır: `[m]` `[s]` `[p]` `[q]` (işlev tuşları, simgeli), `[P]` (p'yi ~1 s basılı tut), `[0]`…`[9]` `[*]` `[#]`, `[PTT]`. LCD kareleri `<span class="lcd" data-t="KANAL 01" data-c="01">` |
| `uret.py` | `kaynak.html` → `kullanim_kilavuzu.html` |

Ekran metinleri firmware'in simülatörde (araç kartı, `disasm/sim`) çalıştırılmasından alındı. Firmware'de ekranda görünen bir şey değişirse kılavuzdaki karşılığı da güncellenmeli.

Yeni sürüm için:

```
cd kilavuz
python3 uret.py
"/Applications/Google Chrome.app/Contents/MacOS/Google Chrome" --headless=new --no-pdf-header-footer \
  --virtual-time-budget=8000 --print-to-pdf="$PWD/KULLANIM_KILAVUZU_AT_V0-31.pdf" "file://$PWD/kullanim_kilavuzu.html"
```

Sürüm numarası `kaynak.html`'de dört yerde geçer (kapak, başlık, açılış ekranı, alt bilgi); PDF adı da ona göre değişir.
