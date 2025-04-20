import math

positions = [
['','','','','','','','',''],
['','','','','','','','',''],
['','','','','','','','',''],
['','','','','','','','',''],
['','','','','','','','',''],
['','','','','','','','',''],
['','','','','','','','',''],
['','','','','','','','',''],
['','','','','','','','','']
]

if True: #INIT VARIABLES
	infinity = math.inf

	score_win = infinity
	score_tile_claimed = 20
	score_two_in_a_row = 3
	score_single_point = 1

	weights = [1,1,1,1,1,1,1,1,1]

	human = 'o'
	ai = 'x'

def CheckBoard(positions):
	full_board = ['','','','','','','','','']
	winner = ''
	for i in range(len(positions)):
		inner_board = positions[i]
		#check each "sub board" and set values to full board if win
		if (inner_board[0] == "x" and inner_board[1] == "x" and inner_board[2] == "x") or (inner_board[3] == "x" and inner_board[4] == "x" and inner_board[5] == "x") or (inner_board[6] == "x" and inner_board[7] == "x" and inner_board[8] == "x") or (inner_board[0] == "x" and inner_board[3] == "x" and inner_board[6] == "x") or (inner_board[1] == "x" and inner_board[4] == "x" and inner_board[7] == "x") or (inner_board[2] == "x" and inner_board[5] == "x" and inner_board[8] == "x") or (inner_board[0] == "x" and inner_board[4] == "x" and inner_board[8] == "x") or (inner_board[2] == "x" and inner_board[4] == "x" and inner_board[6] == "x"):
			full_board[i] = "x"
		elif (inner_board[0] == "o" and inner_board[1] == "o" and inner_board[2] == "o") or (inner_board[3] == "o" and inner_board[4] == "o" and inner_board[5] == "o") or (inner_board[6] == "o" and inner_board[7] == "o" and inner_board[8] == "o") or (inner_board[0] == "o" and inner_board[3] == "o" and inner_board[6] == "o") or (inner_board[1] == "o" and inner_board[4] == "o" and inner_board[7] == "o") or (inner_board[2] == "o" and inner_board[5] == "o" and inner_board[8] == "o") or (inner_board[0] == "o" and inner_board[4] == "o" and inner_board[8] == "o") or (inner_board[2] == "o" and inner_board[4] == "o" and inner_board[6] == "o"):
			full_board[i] = "o"
		else:
			tie = True
			for tile in inner_board:
				if tile == '':
					tie = False
					break
			if tie:
				full_board[i] = 't'

	return full_board

def CheckWinner(full_board):
	if (full_board[0] == "x" and full_board[1] == "x" and full_board[2] == "x") or (full_board[3] == "x" and full_board[4] == "x" and full_board[5] == "x") or (full_board[6] == "x" and full_board[7] == "x" and full_board[8] == "x") or (full_board[0] == "x" and full_board[3] == "x" and full_board[6] == "x") or (full_board[1] == "x" and full_board[4] == "x" and full_board[7] == "x") or (full_board[2] == "x" and full_board[5] == "x" and full_board[8] == "x") or (full_board[0] == "x" and full_board[4] == "x" and full_board[8] == "x") or (full_board[2] == "x" and full_board[4] == "x" and full_board[6] == "x"):
		return 'x'
	elif (full_board[0] == "o" and full_board[1] == "o" and full_board[2] == "o") or (full_board[3] == "o" and full_board[4] == "o" and full_board[5] == "o") or (full_board[6] == "o" and full_board[7] == "o" and full_board[8] == "o") or (full_board[0] == "o" and full_board[3] == "o" and full_board[6] == "o") or (full_board[1] == "o" and full_board[4] == "o" and full_board[7] == "o") or (full_board[2] == "o" and full_board[5] == "o" and full_board[8] == "o") or (full_board[0] == "o" and full_board[4] == "o" and full_board[8] == "o") or (full_board[2] == "o" and full_board[4] == "o" and full_board[6] == "o"):
		return 'o'
	else:
		return ''

def CalculateScore(positions, full_board):
	score = 0
	for i in range(len(positions)):
		inner_board = positions[i]
		inner_board_score = 0
		if full_board[i] == 'x':
			inner_board_score += score_tile_claimed
			score += inner_board_score * weights[i]
			continue
		elif full_board[i] == 'o':
			inner_board_score -= score_tile_claimed
			score += inner_board_score * weights[i]
			continue

		for tile in inner_board:
			if tile == 'x':
				inner_board_score += score_single_point
			elif tile == 'o':
				inner_board_score -= score_single_point

		#check all possible 'two in a row with one empty' and give score accordingly
		if (inner_board[0] == '' and inner_board[1] == 'x' and inner_board[2] == 'x') or (inner_board[0] == '' and inner_board[3] == 'x' and inner_board[6] == 'x') or (inner_board[0] == '' and inner_board[4] == 'x' and inner_board[8] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[1] == '' and inner_board[0] == 'x' and inner_board[2] == 'x') or (inner_board[1] == '' and inner_board[4] == 'x' and inner_board[7] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[2] == '' and inner_board[0] == 'x' and inner_board[1] == 'x') or (inner_board[2] == '' and inner_board[5] == 'x' and inner_board[8] == 'x') or (inner_board[2] == '' and inner_board[4] == 'x' and inner_board[6] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[3] == '' and inner_board[4] == 'x' and inner_board[5] == 'x') or (inner_board[3] == '' and inner_board[0] == 'x' and inner_board[6] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[4] == '' and inner_board[1] == 'x' and inner_board[7] == 'x') or (inner_board[4] == '' and inner_board[3] == 'x' and inner_board[5] == 'x') or (inner_board[4] == '' and inner_board[0] == 'x' and inner_board[8] == 'x') or (inner_board[4] == '' and inner_board[2] == 'x' and inner_board[6] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[5] == '' and inner_board[3] == 'x' and inner_board[4] == 'x') or (inner_board[5] == '' and inner_board[2] == 'x' and inner_board[8] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[6] == '' and inner_board[0] == 'x' and inner_board[3] == 'x') or (inner_board[6] == '' and inner_board[7] == 'x' and inner_board[8] == 'x') or (inner_board[6] == '' and inner_board[2] == 'x' and inner_board[4] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[7] == '' and inner_board[1] == 'x' and inner_board[4] == 'x') or (inner_board[7] == '' and inner_board[6] == 'x' and inner_board[8] == 'x'):
			inner_board_score += score_two_in_a_row
		if (inner_board[8] == '' and inner_board[2] == 'x' and inner_board[5] == 'x') or (inner_board[8] == '' and inner_board[6] == 'x' and inner_board[7] == 'x') or (inner_board[8] == '' and inner_board[0] == 'x' and inner_board[4] == 'x'):
			inner_board_score += score_two_in_a_row

		if (inner_board[0] == '' and inner_board[1] == 'o' and inner_board[2] == 'o') or (inner_board[0] == '' and inner_board[3] == 'o' and inner_board[6] == 'o') or (inner_board[0] == '' and inner_board[4] == 'o' and inner_board[8] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[1] == '' and inner_board[0] == 'o' and inner_board[2] == 'o') or (inner_board[1] == '' and inner_board[4] == 'o' and inner_board[7] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[2] == '' and inner_board[0] == 'o' and inner_board[1] == 'o') or (inner_board[2] == '' and inner_board[5] == 'o' and inner_board[8] == 'o') or (inner_board[2] == '' and inner_board[4] == 'o' and inner_board[6] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[3] == '' and inner_board[4] == 'o' and inner_board[5] == 'o') or (inner_board[3] == '' and inner_board[0] == 'o' and inner_board[6] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[4] == '' and inner_board[1] == 'o' and inner_board[7] == 'o') or (inner_board[4] == '' and inner_board[3] == 'o' and inner_board[5] == 'o') or (inner_board[4] == '' and inner_board[0] == 'o' and inner_board[8] == 'o') or (inner_board[4] == '' and inner_board[2] == 'o' and inner_board[6] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[5] == '' and inner_board[3] == 'o' and inner_board[4] == 'o') or (inner_board[5] == '' and inner_board[2] == 'o' and inner_board[8] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[6] == '' and inner_board[0] == 'o' and inner_board[3] == 'o') or (inner_board[6] == '' and inner_board[7] == 'o' and inner_board[8] == 'o') or (inner_board[6] == '' and inner_board[2] == 'o' and inner_board[4] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[7] == '' and inner_board[1] == 'o' and inner_board[4] == 'o') or (inner_board[7] == '' and inner_board[6] == 'o' and inner_board[8] == 'o'):
			inner_board_score -= score_two_in_a_row
		if (inner_board[8] == '' and inner_board[2] == 'o' and inner_board[5] == 'o') or (inner_board[8] == '' and inner_board[6] == 'o' and inner_board[7] == 'o') or (inner_board[8] == '' and inner_board[0] == 'o' and inner_board[4] == 'o'):
			inner_board_score -= score_two_in_a_row

		score += inner_board_score * weights[i]
	return score

def minimax(positions, selection, depth, alpha, beta, maximizingPlayer):
	board = CheckBoard(positions)
	winner = CheckWinner(board)
	if winner == 'x':
		score = infinity
		return score
	elif winner == 'o':
		score = -infinity
		return score
	elif depth == 0:
		score = CalculateScore(positions, board)
		return score

	if maximizingPlayer:
		maxEval = -infinity
		##-----------------------------------CREATE ALL CHILDREN----------------------------
		children_index = []
		if board[selection] == '':
			children_index.append(selection)
		else:
			for i in range(len(board)):
				if board[i] == '':
					children_index.append(i)
		Break = False
		for index in children_index:
			for i in range(len(positions[index])):
				if positions[index][i] == '':
					positions[index][i] = ai
					select = i
		## ------------------------------------------END-----------------------------------
					Eval = minimax(positions, select, depth - 1, alpha, beta, False)
					positions[index][i] = ''
					maxEval = max(maxEval, Eval)
					alpha = max(alpha, Eval)
					if beta <= alpha:
						Break = True
						break
			if Break:
				break
		return maxEval

	else:
		minEval = infinity
		##-----------------------------------CREATE ALL CHILDREN----------------------------
		children_index = []
		if board[selection] == '':
			children_index.append(selection)
		else:
			for i in range(len(board)):
				if board[i] == '':
					children_index.append(i)
		Break = False
		for index in children_index:
			for i in range(len(positions[index])):
				if positions[index][i] == '':
					positions[index][i] = human
					select = i
		## ------------------------------------------END-----------------------------------
					Eval = minimax(positions, select, depth - 1, alpha, beta, True)
					positions[index][i] = ''
					minEval = min(minEval, Eval)
					beta = min(beta, Eval)
					if beta <= alpha:
						Break = True
						break
			if Break:
				break
		return minEval

def draw(selected_index):
	line_width = 5
	cube_offset = 5
	coloured_cube_offset = 10
	board_offset = 20
	display_offset = 15

	board_length = [int(window_size[0]/3) - board_offset*2, int(window_size[1]/3) - board_offset*2]
	display_length = [int(window_size[0]/3) - display_offset*2, int(window_size[1]/3) - display_offset*2]

	win.fill((50,50,50))

	pygame.draw.line(win, (255,255,255),  (int(window_size[0]/3), 0),(int(window_size[0]/3), window_size[1]), line_width)
	pygame.draw.line(win, (255,255,255),  (int(window_size[0]*2/3), 0),(int(window_size[0]*2/3), window_size[1]), line_width)
	pygame.draw.line(win, (255,255,255),  (0, int(window_size[1]/3)),(window_size[0],int(window_size[1]/3)), line_width)
	pygame.draw.line(win, (255,255,255),  (0, int(window_size[1]*2/3)),(window_size[0],int(window_size[1]*2/3)), line_width)

	currentX = display_offset
	currentY = display_offset
	index = 0

	full_board = CheckBoard(positions)

	for i in range(3):
		for j in range(3):

			if index in selected_index:
				pygame.draw.rect(win, (100,100,100), ((currentX, currentY), display_length))
			if full_board[index] == 'x':
				pygame.draw.rect(win, (40,40,255), ((currentX, currentY), display_length))
			if full_board[index] == 'o':
				pygame.draw.rect(win, (255,40,40), ((currentX, currentY), display_length))

			index += 1
			currentX += int(window_size[0]/3)
		currentY += int(window_size[1]/3)
		currentX = display_offset


	currentX = board_offset
	currentY = board_offset

	full_board_index = 0
	inner_board_index = 0

	for i in range(3):
		for j in range(3):
			for k in range(3):
				for l in range(3):

					pygame.draw.rect(win, (40,40,40), ((currentX + board_length[0] / 3 * l + cube_offset, currentY + board_length[1] / 3 * k + cube_offset), (board_length[0] / 3 - cube_offset*2, board_length[1] / 3 - cube_offset*2)))
					if positions[full_board_index][inner_board_index] == 'x': 
						pygame.draw.rect(win, (40,40,255), ((currentX + board_length[0] / 3 * l + coloured_cube_offset, currentY + board_length[1] / 3 * k + coloured_cube_offset), (board_length[0] / 3 - coloured_cube_offset*2, board_length[1] / 3 - coloured_cube_offset*2)))
					if positions[full_board_index][inner_board_index] == 'o':
						pygame.draw.rect(win, (255,40,40), ((currentX + board_length[0] / 3 * l + coloured_cube_offset, currentY + board_length[1] / 3 * k + coloured_cube_offset), (board_length[0] / 3 - coloured_cube_offset*2, board_length[1] / 3 - coloured_cube_offset*2)))

					inner_board_index += 1
			full_board_index += 1
			inner_board_index = 0
			currentX += int(window_size[0]/3)
		currentY += int(window_size[1]/3)
		currentX = board_offset

		pygame.display.update()


import time
import pygame
pygame.init()

window_size = [720,720]


win = pygame.display.set_mode(window_size)
pygame.display.set_caption("Ultimate-Tic-Tac-Toe-MiniMax")

click = False
selection = 0
click_index = [0,1,2,3,4,5,6,7,8]
draw(click_index)
while True:
	time.sleep(0.1)
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			pygame.quit()
		if event.type == pygame.MOUSEBUTTONDOWN:
			click = True

	if click:
		click = False

		mouseX, mouseY = pygame.mouse.get_pos()

		inner_board_X = int(mouseX / (window_size[0]/9))
		inner_board_Y = int(mouseY / (window_size[1]/9))

		inner_board_X = inner_board_X % 3
		inner_board_Y = inner_board_Y % 3

		inner_board_index = inner_board_Y*3 + inner_board_X

		full_board_X = int(mouseX / (window_size[0]/3))
		full_board_Y = int(mouseY / (window_size[1]/3))
		
		full_board_index = full_board_Y*3 + full_board_X
		
		if positions[full_board_index][inner_board_index] == '' and full_board_index in click_index:
			positions[full_board_index][inner_board_index] = human
			selection = inner_board_index

			##----------------ACTIVATE MINIMAX-------------------
			board = CheckBoard(positions)
			maxEval = -infinity
			children_index = []
			if board[selection] == '':
				children_index.append(selection)
			else:
				for i in range(len(board)):
					if board[i] == '':
						children_index.append(i)
			draw(children_index)
			Break = False

			for index in children_index:
				for i in range(len(positions[index])):
					if positions[index][i] == '':
						positions[index][i] = ai
						select = i

						Eval = minimax(positions, select, 6, -infinity, infinity, False)
						positions[index][i] = ''
						if Eval > maxEval:
							maxEval = Eval
							bestMove = [index, i]

			selection = bestMove[1]
			positions[bestMove[0]][bestMove[1]] = ai
						
		board = CheckBoard(positions)
		click_index = []

		if board[selection] == '':
			click_index.append(selection)
		else:
			for i in range(len(board)):
				if board[i] == '':
					click_index.append(i)
		draw(click_index)
