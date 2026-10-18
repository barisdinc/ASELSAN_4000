/* jsc mkee.js -- out.bin : writes the default EEPROM image */
load('m7700.js'); load('board.js');
const ee = globalThis.Board.defaultEeprom();
writeFile(arguments[0], ee);
print('written', ee.length, globalThis.Board.eeCheckSums(ee));
