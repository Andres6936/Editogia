#include "Editogia/Util/stringfunc.h"
#include "Editogia/Render/View/window.h"
#include <sstream>
#include <math.h> // digits_in() uses log10(); move_decimal() uses pow()

std::vector<std::string> break_into_lines(std::string text, int linesize)
{
	std::vector<std::string> ret;

	size_t chars = 0; // Number of actually-printed characters at...
	size_t pos = 0; // ... this point in the string
	size_t linebreak = std::string::npos; // The last acceptable breakpoint
	std::string active_color_tag;
	while ((text.length() > linesize || text.find('\n') != std::string::npos) &&
		   pos < text.size())
	{
		bool force = false;
		if (text.substr(pos, 3) == "<c=")
		{
			size_t tmppos = text.find('>', pos);
			if (tmppos == std::string::npos)
			{
				//debugmsg("Bad colortag!");
				return ret;
			}
			else
			{
				active_color_tag = text.substr(pos, tmppos - pos + 1);
				pos = tmppos;
			}
			linebreak = pos;
			chars--;
		}
		else if (text[pos] == '\n')
		{
			linebreak = pos;
			force = true;
		}
		else if (text[pos] == ' ')
			linebreak = pos;
		pos++;
		chars++;
		if (force || chars > linesize)
		{
			std::string tmp;
			if (linebreak == std::string::npos)
			{
				linebreak = linesize - 1;
				tmp = text.substr(0, linebreak) + "-";
				text = text.substr(linebreak);
			}
			else if (text[linebreak] == '\n' || text[linebreak] == ' ')
			{
				tmp = text.substr(0, linebreak);
				text = text.substr(linebreak + 1);
// Ostensibly for color tags, but could cause a problem?
			}
			else if (text[linebreak] == '>')
			{
				linebreak++;
				tmp = text.substr(0, linebreak);
				text = text.substr(linebreak);
			}
			ret.push_back(tmp);
			if (!active_color_tag.empty())
			{
				text = active_color_tag + text;
			}
			pos = 0;
			chars = 0;
			linebreak = std::string::npos;
		}
	}
	if (!text.empty())
	{
		ret.push_back(text);
	}
	return ret;
/*
  size_t linebreak = text.find_last_of(" ", linesize);
  std::string tmp;
  if (linebreak == std::string::npos) {
   linebreak = linesize - 1;
   tmp = text.substr(0, linebreak) + "-";
  } else
   tmp = text.substr(0, linebreak);

  ret.push_back(tmp);
  text = text.substr(linebreak + 1);
 }

 ret.push_back(text);
*/

	return ret;
}

std::string load_to_delim(std::istream& datastream, std::string delim)
{
	std::string ret, tmp;
	do
	{
		datastream >> tmp;
		if (tmp != delim)
			ret += tmp + " ";
	} while (tmp != delim && !(datastream.eof()));

	if (!ret.empty() && ret[ret.size() - 1] == ' ')
		ret = ret.substr(0, ret.size() - 1);

	return ret;
}

std::string trim(const std::string& orig)
{
	std::string ret = orig;
	int front = 0, back = ret.length() - 1;
	while (front < ret.length() &&
		   (ret[front] == ' ' || ret[front] == '\n' || ret[front] == '\t'))
	{
		front++;
	}

	ret = ret.substr(front);

	back = ret.length() - 1;

	while (back >= 0 &&
		   (ret[back] == ' ' || ret[back] == '\n' || ret[back] == '\t'))
	{
		back--;
	}

	ret = ret.substr(0, back + 1);

	return ret;
}

std::string no_caps(const std::string& orig)
{
	std::string ret = orig;
	for (int i = 0; i < ret.length(); i++)
	{
		if (ret[i] >= 'A' && ret[i] <= 'Z')
			ret[i] += 'a' - 'A';
	}

	return ret;
}

std::string capitalize(const std::string& orig)
{
	std::string ret = orig;
	size_t tagpos = orig.find("<c="); // Find the first tag
	size_t start; // Used below
	if (tagpos != std::string::npos)
	{
		for (int i = 0; i < tagpos; i++)
		{  // Can we capitalize before the tag?
			if (ret[i] >= 'a' && ret[i] <= 'z')
			{
				ret[i] += 'A' - 'a';  // Capitalize!
				return ret;
			}
			else if (ret[i] != ' ')
			{
				return ret; // We're already capitalized!
			}
		}
// If we reach this point, we found a tag but there's nothing before it.
		start = orig.find(">", tagpos);
		start++;
	}
	else
	{  // No tags - start from the beginning of the string
		start = 0;
	}
	for (int i = start; i < ret.size(); i++)
	{
		if (ret[i] >= 'a' && ret[i] <= 'z')
		{
			ret[i] += 'A' - 'a';
			return ret;
		}
		else if (ret[i] != ' ')
		{
			return ret; // We're already capitalized!
		}
	}
	return ret; // All blank spaces??
}

std::string capitalize_all_words(const std::string& orig)
{
	std::string ret = orig;

	for (int i = 0; i < ret.size(); i++)
	{
		std::string tag_check = no_caps(ret.substr(i, 3));
		if (tag_check == "<c=")
		{  // It's a tag!
// Advance until we find the end of the tag.
			while (ret[i] != '>' && i < ret.size())
			{
				i++;
			}
			if (i >= ret.size())
			{  // Unterminated tag, oh well
				return ret;
			}
		}
		if (ret[i] >= 'a' && ret[i] <= 'z' && (i == 0 || !is_letter(ret[i - 1])))
		{
			ret[i] = ret[i] - 'a' + 'A';
		}
	}

	return ret;
}

std::string remove_color_tags(const std::string& orig)
{
	std::string ret;
	bool in_tag = false;
	for (int i = 0; i < orig.size(); i++)
	{
		if (in_tag)
		{
			if (orig[i] == '>')
			{
				in_tag = false;
			}
		}
		else
		{
			if (orig[i] == '<')
			{
				in_tag = true;
			}
			else
			{
				ret += orig[i];
			}
		}
	}
	return ret;
}

int tagless_length(const std::string& orig)
{
	std::string tagless = remove_color_tags(orig);
	return tagless.length();
}

std::string itos(int num)
{
	std::stringstream ret;
	ret << num;
	return ret.str();
}

int digits_in(int num)
{
	if (num == 0)
	{
		return 1; // log10() chokes on 0
	}
	if (num < 0)
	{
		return 1 + digits_in(0 - num);  // 1 extra for the - character
	}

	return 1 + log10(num);  // 1 extra since log10 is logarythmic
}

std::string move_decimal(int num, int moves)
{
	std::stringstream ret;
	int divisor = int(pow(10, moves));
	int decimal = num % divisor;
	ret << num / divisor << ".";
// We may need to add 0s so that "108" doesn't become "1.8"
	for (int power = moves - 1; power >= 1; power--)
	{
		int val = int(pow(10, power));
		if (decimal < val)
		{
			ret << "0";
		}
		else
		{
			power = 0;
		}
	}
	ret << decimal;
	return ret.str();
}


std::string letters_after(long letter, bool vowel_before)
{
// Force lowercase.
	if (letter >= 'A' && letter <= 'Z')
	{
		letter = letter + 'a' - 'A';
	}

//abcdefghijklmnopqrstuvwxyz
	if (vowel_before)
	{
		if (is_vowel(letter))
		{
			return "bcdfghklmnpqrstvwxyz";
		}
		else if (letter == 'y')
		{
			return "abcdefgiklmnoprstuz";
		}
		else
		{
			return "abcdefghijklmnopqrstuvwyz";
		}
	}
	else
	{
		switch (letter)
		{
		case 'a':
			return "bcdefghiklmnpqrstuvwxyz";
		case 'b':
			return "aeiloruy";
		case 'c':
			return "aehiloru";
		case 'd':
			return "aeioruy";
		case 'e':
			return "abcdefghilmnpqrstuvwyz";
		case 'f':
			return "aeiloruy";
		case 'g':
			return "aeiloru";
		case 'h':
			return "aeiou";
		case 'i':
			return "abcdefglmnopqrstuvz";
		case 'j':
			return "aeiou";
		}
	}
	return "a";
}

bool is_letter(char ch)
{
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

bool is_vowel(char ch)
{
// TODO: And sometimes y?
	return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
			ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U');
}
