	//THROWDBG(*(S64*)((U8*)from_nodes + 0x08) != *(S64*)((U8*)to_nodes + 0x08) || *(S64*)((U8*)to_nodes + 0x08) != *(S64*)((U8*)values + 0x08), EXCPT_DBG_ARG_OUT_DOMAIN);
	//THROWDBG(node_num <= 0 || begin_node < 0 || node_num <= begin_node, EXCPT_DBG_ARG_OUT_DOMAIN);
	const S64* from_nodes2 = \{from_nodes_.id}->B;
	const S64* to_nodes2 = \{to_nodes_.id}->B;
	const S64* values2 = \{values_.id}->B;
	S64 len = \{from_nodes_.id}->L;
	S64 i;
	//for (i = 0; i < len; i++)
	//	THROWDBG(from_nodes2[i] < 0 || node_num <= from_nodes2[i] || to_nodes2[i] < 0 || node_num <= to_nodes2[i] || values2[i] < 0, EXCPT_DBG_ARG_OUT_DOMAIN);

	Array_<int64_t> *result=new Array_<int64_t>();
	result->L = node_num;
	result->B = new int64_t[node_num+1];
	S64* distance = result->B;
	for (i = 0; i < node_num; i++)
		distance[i] = LLONG_MAX;
	distance[\{begin_nodes_.id}] = 0;

	S64* heap = (S64*)malloc(sizeof(S64) * (2 * (size_t)(node_num * len) + 2));
	int heap_cnt = 1;
	heap[0] = begin_node;
	heap[1] = 0;
	while (heap_cnt > 0)
	{
		S64 item_node = heap[0];
		S64 item_value = heap[1];
		heap_cnt--;
		heap[0] = heap[heap_cnt * 2];
		heap[1] = heap[heap_cnt * 2 + 1];
		{
			S64 del_idx = 0;
			for (; ; )
			{
				if ((del_idx + 1) * 2 - 1 < heap_cnt && heap[del_idx * 2 + 1] > heap[((del_idx + 1) * 2 - 1) * 2 + 1])
				{
					S64 tmp;
					tmp = heap[del_idx * 2];
					heap[del_idx * 2] = heap[((del_idx + 1) * 2 - 1) * 2];
					heap[((del_idx + 1) * 2 - 1) * 2] = tmp;
					tmp = heap[del_idx * 2 + 1];
					heap[del_idx * 2 + 1] = heap[((del_idx + 1) * 2 - 1) * 2 + 1];
					heap[((del_idx + 1) * 2 - 1) * 2 + 1] = tmp;
					del_idx = (del_idx + 1) * 2 - 1;
				}
				else if ((del_idx + 1) * 2 < heap_cnt && heap[del_idx * 2 + 1] > heap[((del_idx + 1) * 2) * 2 + 1])
				{
					S64 tmp;
					tmp = heap[del_idx * 2];
					heap[del_idx * 2] = heap[((del_idx + 1) * 2) * 2];
					heap[((del_idx + 1) * 2) * 2] = tmp;
					tmp = heap[del_idx * 2 + 1];
					heap[del_idx * 2 + 1] = heap[((del_idx + 1) * 2) * 2 + 1];
					heap[((del_idx + 1) * 2) * 2 + 1] = tmp;
					del_idx = (del_idx + 1) * 2;
				}
				else
					break;
			}
		}
		if (distance[item_node] < item_value)
			continue;
		for (i = 0; i < len; i++)
		{
			if (from_nodes2[i] != item_node)
				continue;
			if (distance[to_nodes2[i]] > distance[item_node] + values2[i])
			{
				distance[to_nodes2[i]] = distance[item_node] + values2[i];
				heap[heap_cnt * 2] = to_nodes2[i];
				heap[heap_cnt * 2 + 1] = distance[to_nodes2[i]];
				{
					S64 ins_idx = heap_cnt;
					for (; ; )
					{
						if (ins_idx > 0 && heap[ins_idx * 2 + 1] < heap[(ins_idx - 1) / 2 * 2 + 1])
						{
							S64 tmp;
							tmp = heap[ins_idx * 2];
							heap[ins_idx * 2] = heap[(ins_idx - 1) / 2 * 2];
							heap[(ins_idx - 1) / 2 * 2] = tmp;
							tmp = heap[ins_idx * 2 + 1];
							heap[ins_idx * 2 + 1] = heap[(ins_idx - 1) / 2 * 2 + 1];
							heap[(ins_idx - 1) / 2 * 2 + 1] = tmp;
							ins_idx = (ins_idx - 1) / 2;
						}
						else
							break;
					}
				}
				heap_cnt++;
			}
		}
	}
	free(heap);

	return result;
