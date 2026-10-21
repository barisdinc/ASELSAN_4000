/* surum_notlari.js - sürüm notlarını HTML'e çevirir (yukle.html "Neler değişti", surumler.html).
 * Kaynak: ozel_firmware/degisiklikler.json -> mkfirmware.py -> window.FIRMWARE_NOTES (firmware.js). */
(function () {
  'use strict';
  const esc = s => String(s).replace(/[&<>"]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;' }[c]));
  const notNo = s => { const [a, b] = s.split('-').map(Number); return a * 1000 + b; };
  /* "AT V0-16" -> { on: 'AT', no: 16, ad: '0-16' } */
  const surumNo = v => { const m = /^(\S\S) V(\d+)-(\d+)$/.exec(v || ''); return m ? { on: m[1], no: +m[2] * 1000 + +m[3], ad: m[2] + '-' + m[3] } : null; };
  const TURLER = [['yeni', 'Yeni'], ['duzeltme', 'Düzeltme'], ['degisiklik', 'Değişiklik']];
  const tarih = t => t.split('-').reverse().join('.');

  function etiket(n) {
    return n.geri_alindi ? '<span class="etiket note">geri alındı</span>'
      : n.durum === 'gercek' ? '<span class="etiket ok">gerçek telsizde denendi</span>'
      : '<span class="etiket warn">yalnız simülatörde denendi</span>';
  }
  function surumHtml(n, on) {
    let li = '';
    for (const [k, ad] of TURLER) for (const d of n[k] || []) li += '<li><span class="tur tur-' + k + '">' + ad + '</span> ' + esc(d) + '</li>';
    return '<div class="surumnot' + (n.geri_alindi ? ' eski' : '') + '" id="v' + n.surum + '"><div class="surum">' + esc((on || 'AT') + ' V' + n.surum) +
      ' <span class="note">· ' + tarih(n.tarih) + '</span>' + etiket(n) + '</div><ul>' + li + '</ul></div>';
  }
  function ozellikHtml(not, on) {
    if (!not.ozellikler || !not.ozellikler.length) return '';
    return '<dl class="ozellik">' + not.ozellikler.map(o => '<dt>' + esc(o.baslik) + ' <span class="note">· ' + esc((on || 'AT') + ' V' + o.surum) + ' ile geldi</span></dt><dd>' + esc(o.aciklama) + '</dd>').join('') + '</dl>';
  }
  const CSS = '.surumnot .surum { margin: 10px 0 2px; font-weight: 600; } .surumnot .surum .note { font-weight: 400; }' +
    '.surumnot ul { margin: 2px 0 0; padding-left: 18px; } .surumnot li { margin: 3px 0; }' +
    '.surumnot.eski { color: var(--muted); } .surumnot.eski li { text-decoration: line-through; }' +
    '.etiket { display: inline-block; font-size: 11px; font-weight: 600; border-radius: 4px; padding: 0 6px; margin-left: 6px; border: 1px solid currentColor; }' +
    '.tur { display: inline-block; min-width: 72px; font-size: 11px; font-weight: 600; color: var(--muted); }' +
    '.tur-yeni { color: var(--good); } .tur-duzeltme { color: var(--bad); } .tur-degisiklik { color: var(--accent); }' +
    '.ozellik dt { font-weight: 600; margin-top: 8px; } .ozellik dt .note { font-weight: 400; } .ozellik dd { margin: 2px 0 0 0; }';
  const st = document.createElement('style'); st.textContent = CSS; document.head.appendChild(st);
  window.SurumNotlari = { esc, notNo, surumNo, surumHtml, ozellikHtml, NOTLAR: window.FIRMWARE_NOTES || {} };
})();
