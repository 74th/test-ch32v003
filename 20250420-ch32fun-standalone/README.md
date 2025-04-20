# ch32fun をスタンドアローンで動かす

ch32funをチェックアウトしたGitリポジトリに依存しない形でコピーして動かすための方法。

## 手順

### minichlinkをビルドし、パスの通ったところに置いておく

```
git clone https://github.com/cnlohr/ch32fun.git
cd ch32fun/minichlink
make all
cp minichlink /usr/local/bin/
```

### ファイルコピー

```bash
CH32FUN=/Users/nnyn/ghq/github.com/cnlohr/ch32fun
cp -rf ${CH32FUN}/examples/template/* ${CH32FUN}/examples/template/.* ./

mkdir ch32fun
cp ${CH32FUN}/ch32fun/ch32fun.* ./ch32fun/
cp ${CH32FUN}/ch32fun/ch32v003hw.h ./ch32fun/
cp -rf ${CH32FUN}/LICENSE ./ch32fun/

mkdir misc
cp ${CH32FUN}/misc/libgcc.a ./misc/
cp ${CH32FUN}/misc/LIBGCC_LICENSE ./misc/
```

### 書き換え箇所

#### Makefile

- minichlinkのパスを指定する
- ch32funのパスを指定する

```makefile
MINICHLINK:=$(shell dirname $(shell which minichlink))

include ./ch32fun/ch32fun.mk
```

#### ch32fun/ch32fun.mk

書き込みのコマンドにて、MINICHLINKをビルドするようになっているので除外する

```Makefile
cv_flash : $(TARGET).bin
#	make -C $(MINICHLINK) all
	$(FLASH_COMMAND)
```
