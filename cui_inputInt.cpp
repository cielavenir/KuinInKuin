wchar_t buf[33];
int ptr = 0;
buf[0] = L'\0';
for (; ; )
{
	wchar_t c = ReadIo_();
	if (c == L'\r')
		continue;
	if (c == WEOF)
	{
		if (buf[0] == L'\0')
			THROW(EXCPT_INVALID_DATA_FMT);
		break;
	}
	if (c == L'\0')
	{
		if (buf[0] == L'\0')
			continue;
		break;
	}
	if (ptr == 32)
		THROW(EXCPT_INVALID_DATA_FMT);
	buf[ptr] = c;
	ptr++;
}
buf[ptr] = L'\0';
{
	S64 result;
	wchar_t* ptr2;
	result = wcstoll(buf, &ptr2, 10);
	if (*ptr2 != L'\0')
		THROW(EXCPT_INVALID_DATA_FMT);
	return result;
}
