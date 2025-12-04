-- Ğ¡ºì°ü¹¦ÄÜ		ºì°üÊ¹ÓÃ
-- By Spe(2004-01-01)

function main()
	exp=GetExp()
	
	if (exp < 0) then
		exp = exp*-1
		AddOwnExp(exp)
		Msg2Player("Sau khi nghiªn cøu kü B¸t Nh­îc T©m Kinh, b¹n c¶m thÊy toµn th©n khoan kho¸i, kinh nghiÖm thùc chiÕn håi phôc kh«ng İt. ")
		return 0
	end
	Msg2Player("B¹n nghiªn cøu B¸t Nh­îc T©m Kinh ®­îc nöa ngµy, nh­ng vÉn ch­a lÜnh ngé ®­îc g×. ")
	return 1
	
end
