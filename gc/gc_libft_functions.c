#include "garbage_collector.h"
#include "../libft/libft.h"

char	*gc_strdup(const char *s1)
{
	char		*store;
	size_t		i;
	int			len;
	const char	*temp;

	i = 0;
	len = 0;
	temp = s1;
	while (*temp != '\0')
	{
		temp++;
		len++;
	}
	store = ft_malloc(len + 1, sizeof(char));
	if (!store)
		return (NULL);
	while (s1[i] != '\0')
	{
		store[i] = s1[i];
		i++;
	}
	store[i] = '\0';
	return (store);
}

char	*gc_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	str_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (gc_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	substr = ft_malloc(len + 1, sizeof(char));
	if (!substr)
		return (NULL);
	ft_strlcpy(substr, s + start, len + 1);
	return (substr);
}

char	**gc_split(char const *s, char c)
{
	char	**str;
	int		word_count;

	if (!s)
		return (NULL);
	if (*s == '\0')
	{
		str = ft_malloc(1, sizeof(char *));
		if (!str)
			return (NULL);
		str[0] = NULL;
		return (str);
	}
	word_count = gc_count_words(s, c);
	str = ft_malloc(word_count + 1, sizeof(char *));
	if (!str)
		return (NULL);
	if (!gc_fill_words(str, s, c))
	{
		return (NULL);
	}
	return (str);
}

int	gc_count_words(char const *s, char c)
{
	int	counter;
	int	in_word;

	counter = 0;
	in_word = 0;
	while (*s != '\0')
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			counter++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (counter);
}

int	gc_fill_words(char **str, const char *s, char c)
{
	const char	*start;
	char		*temp;
	int			i;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			start = s;
			while (*s && *s != c)
				s++;
			temp = gc_split_substr(start, s);
			if (!temp)
			{
				gc_free_words(str, i);
				return (0);
			}
			str[i++] = temp;
		}
	}
	str[i] = NULL;
	return (1);
}

char	*gc_split_substr(char const *start, char const *end)
{
	int		len;
	char	*word;

	len = end - start;
	word = ft_malloc(len + 1, sizeof(char));
	if (!word)
		return (NULL);
	ft_memcpy(word, start, len);
	return (word[len] = '\0', word);
}
void	gc_free_words(char **str, int i)
{
	while (i--)
		free(str[i]);
	free(str);
}