typedef struct LIST {
	int r,g,b;				// RGB値
	int num;				// 画素数
	struct LIST *next;	// ポインタチェーン
} List;
