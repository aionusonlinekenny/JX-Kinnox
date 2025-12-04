--Create by yfeng ,2004-3-9
--ÌúÂŞººÓÃÀ´ÑĞÏ°£¬¿ÉÒÔÔö¼ÓÊµÕ½¾­Ñé£¬
--ÒÔÃ¿10¼¶ÎªÒ»¸ö¶Î£¬ËùÔö¼ÓµÄ¾­ÑéÎª¸Ã¼¶±ğ¶ÎÄÚÍæ¼ÒÉı¼¶ËùĞè¾­Ñé
--µÄ×ÜºÍµÄÆ½¾ùÖµµÄ1/10,²¢°´Ò»¶¨µÄ¼¸ÂÊ»ñµÃÕâ1/10µÄ¾­Ñé£¬·ñÔò£¬»ñµÃÉÙ²¿·Ö¾­Ñé
--¾ßÌå£º10¼¶¶ÎÄÚ100%»ñµÃ1/10¾­Ñé£¬100¼¶¶ÎÄÚ5%»ñµÃ1/10¾­Ñé
--ÆäËûÇé¿ö×î¶à²»ÄÜ»ñµÃ³¬¹ı1/10¾­ÑéµÄ50%
--1¼¶ÒÔÏÂ°´1¼¶Ëã£¬100¼¶ÒÔÉÏ£¬°´100¼¶Ëã
--¸ÃÎïÆ·²»ÏŞÖÆÊ¹ÓÃ´ÎÊı£¬µ«¼ÍÂ¼Ê¹ÓÃµÄ´ÎÊı
--ÈÎÎñ±äÁ¿82ÓÃÓÚ¼ÇÂ¼¸ÃÎïÆ·µÄÊ¹ÓÃ´ÎÊı
--Èç¹û³¬¹ıÕâ¸öÊı×Ö£¬Ôò²»ÄÜÊ¹ÓÃ¸ÃÎïÆ·ÁË¡£
function main(sel)
	exps={
		{430,100},
		{2960,90},
		{9490,80},
		{22515,70},
		{48190,50},
		{104980,30},
		{246310,20},
		{609360,15},
		{1511850,10},
		{3611636,5}
	}
	msg={
		"B¹n ®Êu víi ThiÕt La H¸n h¬n nöa ngµy, kÕt qu¶ ®· thu ®­îc mét sè kinh nghiÖm thùc tiÔn chiÕn ®Êu ",
		"B¹n ®Êu víi ThiÕt La H¸n h¬n nöa ngµy, kÕt qu¶ ®· thu ®­îc mét sè kinh nghiÖm thùc tiÔn chiÕn ®Êu ",
		"B¹n cïng víi ThiÕt La H¸n ®èi luyÖn h¬n nöa ngµy, kÕt qu¶ kh«ng thu ®­îc hiÖu qu¶ g× "
	}
	
	num = GetTask(82) --È¡Ê¹ÓÃ´ÎÊı£¬¿¿Ã»ÓĞÈËÄÜÊ¹ÓÃ40¶àÒÚ´Î°É£¬²»×ö¾À´í´¦ÀíÁË
	level = GetLevel()
	if(level <1) then 
		level =1
	end
	if(level > 130) then
		Msg2Player(msg[3])
		return 1
	end
	if(level >100) then
		level = 100
	end	
	levelseg = floor((level-1)/10)+1
	rnd = random(100)
	if(rnd > exps[levelseg][2]) then --»ñµÃÉÙĞí¾­Ñé
		AddOwnExp(floor(exps[levelseg][1]*rnd/200))
		Msg2Player(msg[1])
		SetTask(82,num+1)
		return 0
	end
	--ÃüºÃ£¬»ñµÃ²»ÉÙ¾­Ñé
	AddOwnExp(exps[levelseg][1])
	Msg2Player(msg[2])
	SetTask(82,num+1)
	return 0
end