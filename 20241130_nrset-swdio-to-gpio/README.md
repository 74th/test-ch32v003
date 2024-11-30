# NRSTとSWDIOをGPIOとして使う

NRST は INPUT としてしか使えない様子。デバッガでセットする。

SWDIO は INPUT/OUTPUT のどちらでも使える。ファームウェア中でセットする。

## NRSTをGPIOにする

### 有効化

```
minichlink -d
```

### 無効化

```
minichlink -D
```

## SWDIOをGPIOにする

### 有効化

```c
// AFIOを有効化
RCC->APB2PCENR |= RCC_APB2Periph_AFIO;

// AFIOでSWDIOをDisable
AFIO->PCFR1 |= AFIO_PCFR1_SWJ_CFG_DISABLE;
```

### 無効化

有効化すると書き込めなくなるため、以下のコマンドを実行中に電源オフ→オンにしてファームウェアを消去する。

```
minichlink -u
```