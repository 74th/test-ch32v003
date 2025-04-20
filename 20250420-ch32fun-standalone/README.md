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
cp ${CH32FUN}/misc/CH32V003xx.svd ./misc/
cp ${CH32FUN}/misc/LIBGCC_LICENSE ./misc/
```

### プロジェクト名の変更

```bash
mv template.c standalone.c
```

### Makefileの書き換え

- TARGETをプロジェクト名に変更
- minichlinkのパスを指定する
- ch32funのパスを指定する

```makefile
# Makefile
all : flash

TARGET:=template

TARGET_MCU?=CH32V003
include ../../ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
```

↓

```makefile
# Makefile
all : flash

TARGET:=standalone

TARGET_MCU?=CH32V003

MINICHLINK:=$(shell dirname $(shell which minichlink))

include ./ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
```

### ch32fun/ch32fun.mkの書き換え

書き込みのコマンドにて、MINICHLINKをビルドするようになっているので除外する

```makefile
cv_flash : $(TARGET).bin
	make -C $(MINICHLINK) all
	$(FLASH_COMMAND)

```

↓

```Makefile
cv_flash : $(TARGET).bin
#	make -C $(MINICHLINK) all
	$(FLASH_COMMAND)
```

### VS Codeの設定の書き換え

- .vscode/c_cpp_properties.json
  - `"${workspaceFolder}/../../ch32fun"` と指定される行を、`"${workspaceFolder}/ch32fun"` に変更
- .vscode/launch.json
  - `"svdPath": "${workspaceFolder}/../../misc/CH32V003xx.svd"` と指定される行を、`"${workspaceFolder}/misc/CH32V003xx.svd"` に変更

## おまけ、git submoduleで行う方法

Makefileのincludeの部分を書き換えるだけで良い。

```bash
git submodule add https://github.com/cnlohr/ch32fun.git ch32fun
CH32FUN=$(PWD)/ch32fun

cp -rf ch32fun/examples/template/* ch32fun/examples/template/.* ./
```

```makefile
# Makefile

```makefile
# Makefile
all : flash

TARGET:=template

TARGET_MCU?=CH32V003
include ../../ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
```

↓

```makefile
# Makefile
all : flash

TARGET:=standalone

TARGET_MCU?=CH32V003

include ./ch32fun/ch32fun/ch32fun.mk

flash : cv_flash
clean : cv_clean
```
