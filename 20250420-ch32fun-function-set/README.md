# 計算命令別の容量

|                     | Printf未使用(B) | Printf使用(B) |
| ------------------- | ------------: | --------------: |
| 以下の処理なし      |           588 |           1,916 |
| uint32_tの乗算      |           624 |           1,948 |
| uint32_tの除算      |           728 |           1,924 |
| uint32_tの乗算/除算 |           760 |           1,956 |
| floatの乗算         |         1,724 |           3,228 |
| floatの除算         |         1,960 |           3,332 |
| floatの乗算/除算    |         2,592 |           3,972 |
