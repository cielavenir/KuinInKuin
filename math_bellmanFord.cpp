		//THROWDBG(*(S64*)((U8*)from_nodes + 0x08) != *(S64*)((U8*)to_nodes + 0x08) || *(S64*)((U8*)to_nodes + 0x08) != *(S64*)((U8*)values + 0x08), EXCPT_DBG_ARG_OUT_DOMAIN);
		//THROWDBG(node_num <= 0 || begin_node < 0 || node_num <= begin_node, EXCPT_DBG_ARG_OUT_DOMAIN);
		const S64* from_nodes2 = \{from_nodes_.id}->B;
		const S64* to_nodes2 = \{to_nodes_.id}->B;
		const S64* values2 = \{values_.id}->B;
		S64 len = \{from_nodes_.id}->L;
		S64 i;
		//for (i = 0; i < len; i++)
		//	THROWDBG(from_nodes2[i] < 0 || node_num <= from_nodes2[i] || to_nodes2[i] < 0 || node_num <= to_nodes2[i], EXCPT_DBG_ARG_OUT_DOMAIN);

		Array<int64_t> *result=new Array<int64_t>();
		result->L = node_num;
		result->B = new inr64_t[node_num+1];
		S64* distance = result->B;
		for (i = 0; i < node_num; i++)
			distance[i] = LLONG_MAX;
		distance[begin_node] = 0;

		Bool found;
		do
		{
			found = False;
			for (i = 0; i < len; i++)
			{
				S64 from_distance = distance[from_nodes2[i]];
				if (from_distance != LLONG_MAX && distance[to_nodes2[i]] > from_distance + values2[i])
				{
					distance[to_nodes2[i]] = from_distance + values2[i];
					found = True;
				}
			}
		} while (found);
		return result;
