wchar_t c = L'\0';
for (; ; )
{
	c = ReadIo_();
	if (c == L'\r')
		continue;
	if (c == WEOF)
		THROW(EXCPT_INVALID_DATA_FMT);
	if (c != L'\0')
		break;
}
return c;
