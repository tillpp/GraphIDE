# TextComponent

TextComponent
	has getWidth,getHeight
	has getYOffset // up from baseline

TextUnit	TU		:TextComponent
	has Text
[]TextReverse	TR		:TextComponent
	has Text
[]TextAlignment	TA		:TextComponent
	has Text
	enum{right,center,left} 
	Note: 	TextSettings has a fullWidth propertie.
[]TextIcon	TI		:TextComponent
	has Text
[]TextScript 	TS		:TextComponent
	has superscript MonoLineText
	has subscript	MonoLineText
[]MathUpDown	MUD		:TextComponent
	has a Symbol
	has superscript	PolyLineText
	has subscript 	PolyLineText
[]MathFrac	MF		:TextComponent
	has up			PolyLineText
	has down 		PolyLineText
MonoLineText MLT
	has multiple TextComponent
	has getHeight: max(ch.height+ch.yoffset)+max(-ch.yoffset)
	has getWidth:  sum(ch.width)	
PolyLineText PLT	
	has multiple MLT
	has getHeight: sum(ch.height)
	has getWidth:  max(ch.width)

TextSettings TSets
	font
	fontsize
	smallscript
	bold,
	italic,
	underline
	strikethrough
	colour
	backgroundcolor
	position x,y

Characterspecificeffects CSE
	function that override character properties
	pre Glyph
	pre draw
	post draw
# Todo

# future
	clean TextUnit (Split code: underline,strikethough into class, extend "AfterApplier" baseclass)

## features
[] Let Text scroll to the left,if the Text doesnt fit into it's boundrie
[] Show Fulltext,when hovering above cut of text.

[] hover event
[] hover to animate: move character up
[] hover to animate: color character
[] hover to open tooltipwindow
[] click event

# cursor stuff todo 
[x] cursor 
[x] cursor movement
[x] writing
[] line too big and break line 
[x] delete (backspace,forward) 
[x] ctrl delete(backspace,forward)
	[x] selecting text
[x] shift
[x] mouse select
[] open stylewindow (to change style of selected text)
[x] copy ctrl+c
[x] paste ctrl+v
[] cut ctrl+x
[] undo ctrl+z
[] redo ctrl+y

