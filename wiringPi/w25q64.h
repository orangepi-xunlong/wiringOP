//#include <arduino.h>
//#include <SPI.h>

// フラッシュメモリ W25Q64の利用開始
void W25Q64_begin(uint8_t cs);
// ステータスレジスタ1の値取得
uint8_t W25Q64_readStatusReg1(void);
// ステータスレジスタ2の値取得
uint8_t W25Q64_readStatusReg2();
// JEDEC ID(Manufacture, Memory Type,Capacity)の取得
void W25Q64_readManufacturer(uint8_t* d);
// Unique IDの取得
void W25Q64_readUniqieID(uint8_t* d);
// 書込み等の処理中チェック
bool W25Q64_IsBusy();
// パワーダウン指定 
void W25Q64_powerDown(void);
// 書込み許可設定
void W25Q64_WriteEnable(void);
// 書込み禁止設定
void W25Q64_WriteDisable(void);
// データの読み込み
uint16_t W25Q64_read(uint32_t addr,uint8_t *buf,uint16_t n);
// 高速データの読み込み
uint16_t W25Q64_fastread(uint32_t addr,uint8_t *buf,uint16_t n);
// セクタ単位消去
bool  W25Q64_eraseSector(uint16_t sect_no, bool flgwait);
// 64KBブロック単位消去
bool  W25Q64_erase64Block(uint16_t blk_no, bool flgwait);
// 32KBブロック単位消去
bool  W25Q64_erase32Block(uint16_t blk_no, bool flgwait);
// 全領域の消去
bool  W25Q64_eraseAll(bool flgwait);
// データの書き込み
uint16_t W25Q64_pageWrite(uint16_t sect_no, uint16_t inaddr, uint8_t* data, uint8_t n);

