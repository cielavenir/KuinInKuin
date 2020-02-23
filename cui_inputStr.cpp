wchar_t stack_buf[STACK_STRING_BUF_SIZE];
wchar_t* buf = stack_buf;
size_t buf_len = STACK_STRING_BUF_SIZE;
size_t len = 0;
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
	buf[len] = c;
	len++;
	if (len == buf_len)
	{
		buf_len += STACK_STRING_BUF_SIZE;
		wchar_t* tmp = (wchar_t*)realloc(buf == stack_buf ? NULL : buf, sizeof(wchar_t) * buf_len);
		if (tmp == NULL)
		{
			if (buf != stack_buf)
				free(buf);
			return NULL;
		}
		if (buf == stack_buf)
			memcpy(tmp, buf, sizeof(wchar_t) * len);
		buf = tmp;
	}
}
buf[len] = L'\0';
Array_<char16_t>* r_ = new Array_<char16_t>();
r_->L = static_cast<int64_t>(len);
r_->B = new char16_t[len + 1];
for (int i_=0;i_<len;i_++) r_->B[i_] = buf[i_];
r_->B[len] = 0;
return r_;
