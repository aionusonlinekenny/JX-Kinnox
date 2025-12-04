Include("\\script\\task\\newtask\\newtask_head.lua")

function main()
	local nWorldMaps = nt_getTask(1027) -- ¿´¿´Íæ¼ÒÉíÉÏÓĞ¶àÉÙ¸öÉ½ºÓÉçğ¢Í¼
	-- ¸øÍæ¼ÒÔö¼Ó100¸öÉ½ºÓÉçğ¢Í¼²ĞÆ¬
	nWorldMaps = nWorldMaps + 100;
	nt_setTask(1027,nWorldMaps);
	Msg2Player("B¹n nhËn ®­îc 100 m¶nh b¶n ®å S¬n Hµ X· T¾c! HiÖn t¹i b¹n cã tæng céng"..nWorldMaps.." m¶nh b¶n ®å S¬n Hµ X· T¾c.");
	return 0
end