	S64 len = \{weights_.id}->Len();
	const S64* weights2 = \{weights_.id}->B;
	const S64* values2 = \{values_.id}->B;
	S64* dp = (S64*)malloc(sizeof(S64) * (size_t)(\{maxWeight_.id} + 1));
	S64 i, j;

	memset(dp, 0, sizeof(S64) * (size_t)(\{maxWeight_.id} + 1));
	if (\{reuse_.id})
	{
		for (i = 0; i < len; i++)
		{
			for (j = weights2[i]; j <= \{maxWeight_.id}; j++)
			{
				S64 value = dp[j - weights2[i]] + values2[i];
				if (dp[j] < value)
					dp[j] = value;
			}
		}
	}
	else
	{
		for (i = 0; i < len; i++)
		{
			for (j = \{maxWeight_.id}; j >= weights2[i]; j--)
			{
				S64 value = dp[j - weights2[i]] + values2[i];
				if (dp[j] < value)
					dp[j] = value;
			}
		}
	}
	S64 result = dp[\{maxWeight_.id}];
	free(dp);
	return result;
