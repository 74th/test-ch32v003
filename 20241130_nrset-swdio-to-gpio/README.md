# NRSTとSWDIOをGPIOとして使う

NRSTはINPUT/OUTPUTのどちらでも使える。デバッガでセットする。

SWDIOはINPUT/OUTPUTのどちらでも使える。ファームウェア中でセットする。

## NRSTをGPIOにする

### 有効化: NRST(Pin4)をPD7(GPIO)にする

```
minichlink -D
```

### 無効化: NRST(Pin4)をNRSTに戻す

```
minichlink -d
```

## SWDIOをGPIOにする

### 有効化: SWDIO(Pin18)をPD1(GPIO)にする

```c
// AFIOを有効化
RCC->APB2PCENR |= RCC_APB2Periph_AFIO;

// AFIOでSWDIOをDisable
AFIO->PCFR1 |= AFIO_PCFR1_SWJ_CFG_DISABLE;
```

### 無効化: SWDIO(Pin18)をSWIDIOに戻す

有効化すると書き込めなくなるため、以下のコマンドを実行中に電源オフ→オンにしてファームウェアを消去する。

```
minichlink -u
```
