	//THROWDBG(fmt == NULL, EXCPT_ACCESS_VIOLATION);
	const char16_t* fmt2 = \{fmt_.id}->B;
	int src_len = \{fmt_.id}->L;
	wchar_t dst[33];
	int dst_ptr = 0;
	int src_ptr = 0;

	dst[dst_ptr] = L'%';
	dst_ptr++;
	if (src_ptr < src_len && (fmt2[src_ptr] == L'+' || fmt2[src_ptr] == L' '))
	{
		dst[dst_ptr] = fmt2[src_ptr];
		dst_ptr++;
		if (dst_ptr + 4 >= 33)
			return NULL;
		src_ptr++;
	}
	if (src_ptr < src_len && (fmt2[src_ptr] == L'-' || fmt2[src_ptr] == L'0'))
	{
		dst[dst_ptr] = fmt2[src_ptr];
		dst_ptr++;
		if (dst_ptr + 4 >= 33)
			return NULL;
		src_ptr++;
	}
	if (src_ptr < src_len && L'1' <= fmt2[src_ptr] && fmt2[src_ptr] <= L'9')
	{
		dst[dst_ptr] = fmt2[src_ptr];
		dst_ptr++;
		if (dst_ptr + 4 >= 33)
			return NULL;
		src_ptr++;
		while (src_ptr < src_len && L'0' <= fmt2[src_ptr] && fmt2[src_ptr] <= L'9')
		{
			dst[dst_ptr] = fmt2[src_ptr];
			dst_ptr++;
			if (dst_ptr + 4 >= 33)
				return NULL;
			src_ptr++;
		}
	}
	if (!(src_ptr < src_len && (fmt2[src_ptr] == L'd' || fmt2[src_ptr] == L'x' || fmt2[src_ptr] == L'X' || fmt2[src_ptr] == L'u' || fmt2[src_ptr] == L'o')))
		return NULL;
    /* Windows 7 (or later) accepts ll (as well as I64).*/
	dst[dst_ptr] = L'l';
	dst_ptr++;
	dst[dst_ptr] = L'l';
	dst_ptr++;
	dst[dst_ptr] = fmt2[src_ptr];
	dst_ptr++;
	dst[dst_ptr] = L'\0';
	dst_ptr++;
	//assert(src_ptr + 1 == src_len);

	wchar_t str[65];
	int len = swprintf(str, 65, dst, \{me_.id});
	//assert(len < 65);
    Array_<char16_t> *result = new Array_<char16_t>();
    result->B = new char16_t[len+1];
    for(int i=0;i<len;i++)result->B[i]=str[i];
    result->B[len]=0;
    result->L=len;
    return result;
