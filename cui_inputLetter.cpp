wchar_t c = fgetwc(stdin);
if (c == WEOF)
	THROW(EXCPT_INVALID_DATA_FMT);
return c;
