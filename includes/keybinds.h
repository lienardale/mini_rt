/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keybinds.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alienard <alienard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 19:01:55 by alienard          #+#    #+#             */
/*   Updated: 2020/03/02 19:30:42 by alienard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBINDS_H
#define KEYBINDS_H

#ifdef Darwin
#define ESC 53
#define MV_R 0
#define MV_L 2
#define MV_BKW 1
#define MV_FWD 13
#define MV_UP 116
#define MV_DWN 121
#define LK_LFT 123
#define LK_RGT 124
#define LK_UP 126
#define LK_DWN 125
#define LK_ZL 43
#define LK_ZR 47
#define FOV_P 69
#define FOV_N 78
#define CHG_RES 15
#define NXT_CAM 49
#endif

#define KEY_BIT_FWD		(1 << 0)
#define KEY_BIT_BKW		(1 << 1)
#define KEY_BIT_LEFT	(1 << 2)
#define KEY_BIT_RIGHT	(1 << 3)
#define KEY_BIT_UP		(1 << 4)
#define KEY_BIT_DOWN	(1 << 5)
#define KEY_BIT_LK_L	(1 << 6)
#define KEY_BIT_LK_R	(1 << 7)
#define KEY_BIT_LK_U	(1 << 8)
#define KEY_BIT_LK_D	(1 << 9)
#define KEY_BIT_LK_ZL	(1 << 10)
#define KEY_BIT_LK_ZR	(1 << 11)
#define MOVE_STEP		0.5
#define LOOK_STEP		0.02

#ifdef Linux
#define ESC 65307
#define MV_R 97
#define MV_L 100
#define MV_BKW 115
#define MV_FWD 119
#define MV_UP 65365
#define MV_DWN 65366
#define LK_LFT 65361
#define LK_RGT 65363
#define LK_UP 65362
#define LK_DWN 65364
#define LK_ZL 44
#define LK_ZR 46
#define FOV_P 65451
#define FOV_N 65453
#define CHG_RES 114
#define NXT_CAM 32
#endif

#endif
