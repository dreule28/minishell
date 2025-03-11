/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dreule <dreule@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:19:15 by dreule            #+#    #+#             */
/*   Updated: 2025/03/11 16:20:06 by dreule           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	isspace(char c)
{
	if (c == ' ' || c == 't' || c == '\n')
		return (0);
	return (1);
}