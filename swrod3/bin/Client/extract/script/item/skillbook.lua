-- ¼¼ÄÜ

-- ItemParticular={MagicID,IniLevel, NeedFaction, BookName, SkillName}
sbook_list = 
	{
		[27]={302, 1,"tangmen", "'Tô TiÔn thuËt. B¹o Vò Lª Hoa' ", 	"B¹o Vò Lª Hoa"},
		[28]={351, 0,"tangmen", "'H·m TÜnh thuËt. Lo¹n Hoµn KÝch' ", 	"Lo¹n Hoµn KÝch"},
		[33]={365, 1,"wudang", "Th¸i Cùc QuyÒn Phæ- QuyÓn 3 ", 	"Thiªn §Þa V« Cùc "},
		[34]={368, 1,"wudang", "'Th¸i Cùc KiÕm Phæ. QuyÓn 2' ", 	"Nh©n KiÕm Hîp NhÊt  "},	
		[35]={361, 1,"tianren", "'V©n Long KÝch. M©u ph¸p' ", 	"V©n Long KÝch"},	
		[36]={362, 1,"tianren", "'L­u Tinh. §ao ph¸p' ", 	"Thiªn Ngo¹i L­u Tinh"},
		[37]={325, 1,"tianwang", "'Thiªn V­¬ng Chïy Ph¸p. QuyÓn 1' ",	"Truy Phong QuyÕt "},
		[38]={323, 1,"tianwang", "Thiªn V­¬ng Th­¬ng ph¸p- QuyÓn 2 ", 	"Truy Tinh Trôc NguyÖt "},
		[39]={322, 1,"tianwang", "Thiªn V­¬ng §ao ph¸p - QuyÓn 3 ",	"Ph¸ Thiªn Tr¶m "},
		[40]={336, 1,"cuiyan", "'Thóy Yªn §ao ph¸p' ", 		"B¨ng Tung V« ¶nh "},
		[41]={337, 1,"cuiyan", "'Thóy Yªn Song ®ao' ", 		"B¨ng T©m Tiªn Tö  "},
		[42]={328, 1,"emei", "'DiÖt KiÕm MËt TÞch' ", 	"Tam Nga TÒ TuyÕt "},
		[43]={380, 1,"emei", "'Nga Mi PhËt Quang Ch­ëng MËt TÞch' ",	"Phong S­¬ng To¸i ¶nh "},
		[45]={339, 1,"tangmen", "'Phi §ao thuËt. NhiÕp Hån NguyÖt ¶nh' ", 	"NhiÕp Hån NguyÖt ¶nh"},
		[46]={342, 1,"tangmen", "'Phi Tiªu thuËt. Cöu Cung Phi Tinh' ", 	"Cöu Cung Phi Tinh"},
		[47]={353, 1,"wudu", "'Ngò §éc Ch­ëng Ph¸p. QuyÓn 1' ", 	"¢m Phong Thùc Cèt "},
		[48]={355, 1,"wudu", "'Ngò §éc §ao ph¸p. QuyÓn 2' ", 	"HuyÒn ¢m Tr¶m "},
		[49]={390, 0,"wudu", "'Ngò §éc NhiÕp T©m thuËt. QuyÓn 3' ", 	"§o¹n C©n Hñ Cèt "},
		[50]={372, 1,"kunlun", "'Ngù Phong thuËt' ", 	"Ng¹o TuyÕt Tiªu Phong "},
		[51]={375, 1,"kunlun", "'Ngù L«i thuËt' ", 	"L«i ®éng Cöu Thiªn "},	
		[52]={394, 0,"kunlun", "'Ngù T©m thuËt' ", 	"Tóy Tiªn T¸ Cèt"},	
		[53]={391, 0,"tianren", "NhiÕp Hån - Tr­íc Chó ", 		"NhiÕp Hån Lo¹n T©m "},
		[54]={357, 1,"gaibang", "'C¸i Bang Ch­ëng Ph¸p' ", 	"Phi Long T¹i Thiªn "},
		[55]={359, 1,"gaibang", "C¸i Bang C«n Ph¸p ", 	"Thiªn H¹ V« CÈu "},	
		[56]={318, 1,"shaolin", "'ThiÕu L©m QuyÒn Ph¸p. QuyÓn 1' ", 	"§¹t Ma §é Giang"},
		[57]={319, 1,"shaolin", "'ThiÕu L©m C«n ph¸p. QuyÓn 2' ", 	"Hoµnh T¶o Lôc Hîp"},		
		[58]={321, 1,"shaolin", "'ThiÕu L©m §ao ph¸p. QuyÓn 3' ", 	"V« T­íng Tr¶m "},	
		[59]={332, 0,"emei", "'Phæ §é MËt TÞch' ",	"Phæ §é Chóng Sinh "},
	};

function LearnSkillByBook(index)
	party = GetLastAddFaction()
	skill = HaveMagic(sbook_list[index][1])
	if(party ~= sbook_list[index][3]) then							-- ²»ÊÇÖ¸¶¨ÃÅÅÉ
		Msg2Player("B¹n cÇm lÊy "..sbook_list[index][4].."Nghiªn cøu c¶ nöa ngµy, kÕt qu¶ còng kh«ng lÜnh ngé ®­îc g× ")
		return 1
	end
	if(GetLevel() < 80) then							-- µ«Î´µ½80¼¶
		Msg2Player("B¹n cÇm lÊy "..sbook_list[index][4].."Nghiªn cøu c¶ nöa ngµy, kÕt qu¶ lÜnh ngé rÊt Ýt. ")
		return 1
	end
	if(skill ~= -1) then							-- ÒÑÑ§¹ý¸Ã¼¼ÄÜ
		Msg2Player("B¹n ®· cÇm "..sbook_list[index][4].."NghiÒn tíi ngÉm lui, nh­ng còng kh«ng häc ®­îc g× trong ®ã ")
		return 1
	end

	AddMagic(sbook_list[index][1],sbook_list[index][2])										-- Ñ§»á¼¼ÄÜ
	
	WriteLog(date("%H%M%S")..": Tµi kho¶n:"..GetAccount()..", nh©n vËt:"..GetName()..", Sö dông: "..sbook_list[index][4].."§· häc ®­îc: "..sbook_list[index][5]);
	Msg2Player("§· häc ®­îc kü n¨ng "..sbook_list[index][5].."'. ")
	return 0
end

function main(sel)
	Genre, DetailType, Particular=GetItemProp(sel)
	return LearnSkillByBook(Particular);
end
