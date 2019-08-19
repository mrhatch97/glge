#include "glge/model_parser/model_parser.h"

#include <array>
#include <fstream>
#include <istream>
#include <limits>
#include <optional>
#include <variant>

namespace glge::model_parser
{
	namespace
	{
		struct FaceGroup
		{
			Index vertex_index;
			std::optional<Index> normal_index;
			std::optional<Index> uv_index;
		};

		struct Face
		{
			std::array<FaceGroup, 3> points;
		};

		class Parser
		{
			std::istream & stream;

			void assert_integrity()
			{
				if (stream.bad())
				{
					throw std::runtime_error(
						EXC_MSG("Stream entered a bad state"));
				}
			}

		public:
			Parser(std::istream & stream) : stream(stream) {}

			template<typename T>
			T expect()
			{
				T extracted;
				stream >> extracted;

				assert_integrity();

				if (stream.fail())
				{
					throw std::runtime_error(
						EXC_MSG("Expected extraction failed"));
				}

				return extracted;
			}

			template<typename T>
			std::optional<T> maybe()
			{
				T extracted;
				stream >> extracted;

				assert_integrity();

				if (stream.fail())
				{
					if (!stream.eof())
					{
						stream.clear();
					}

					return std::nullopt;
				}
				else
				{
					return extracted;
				}
			}

			template<typename T>
			void expect(T expected)
			{
				T extracted = expect<T>();

				if (extracted != expected)
				{
					throw std::runtime_error(
						EXC_MSG("Extracted value did not match expected"));
				}
			}

			template<typename T>
			bool maybe_match(T expected)
			{
				auto position = stream.tellg();

				std::optional<T> extracted = maybe<T>();

				if (!extracted || extracted != expected)
				{
					stream.seekg(position);
					return false;
				}
				else
				{
					return true;
				}
			}

			void ignore(std::streamsize count) { stream.ignore(count); }

			void ignore_until(char delim)
			{
				stream.ignore(std::numeric_limits<std::streamsize>::max(),
							  delim);
			}

			char take()
			{
				if (stream.fail() && !stream.eof())
				{
					throw std::runtime_error(EXC_MSG("wat"));
				}

				char result = stream.get();

				assert_integrity();

				if (stream.fail() && !stream.eof())
				{
					throw std::runtime_error(EXC_MSG("wat"));
				}

				return result;
			}

			bool more_input() { return !stream.eof(); }
		};

		Vertex parse_vertex(Parser & parser)
		{
			float x, y, z;
			x = parser.expect<float>();
			y = parser.expect<float>();
			z = parser.expect<float>();

			return Vertex({x, y, z});
		}

		Normal parse_normal(Parser & parser)
		{
			float x, y, z;
			x = parser.expect<float>();
			y = parser.expect<float>();
			z = parser.expect<float>();

			return Normal({x, y, z});
		}

		TexCoord parse_tex_coord(Parser & parser)
		{
			float u, v;
			u = parser.expect<float>();
			v = parser.expect<float>();

			return TexCoord({u, v});
		}

		FaceGroup parse_face_group(Parser & parser)
		{
			// e.g. 5/3/7 or 5//7 or 5

			const auto vertex_index = Index(parser.expect<size_t>() - 1);

			if (!parser.maybe_match('/'))
			{
				return {vertex_index, std::nullopt, std::nullopt};
			}

			std::optional<Index> uv_index;

			if (parser.maybe_match('/'))
			{
				uv_index = std::nullopt;
			}
			else
			{
				uv_index = Index(parser.expect<size_t>() - 1);
				parser.expect('/');
			}

			// Normal isn't optional at this point - read it
			const auto normal_index = Index(parser.expect<size_t>() - 1);

			return {vertex_index, normal_index, uv_index};
		}

		enum class LineType
		{
			comment,
			empty,
			vertex,
			normal,
			tex_coord,
			face,
			unknown
		};

		LineType parse_line_type(Parser & parser)
		{
			const auto first = parser.take();

			if (!parser.more_input() || first == '\n')
			{
				return LineType::empty;
			}

			const auto second = parser.take();

			if (!parser.more_input() || second == '\n')
			{
				return LineType::empty;
			}

			switch (first)
			{
			case '#':
				return LineType::comment;
			case 'v':
				switch (second)
				{
				case ' ':
					return LineType::vertex;
				case 'n':
					return LineType::normal;
				case 't':
					return LineType::tex_coord;
				default:
					return LineType::unknown;
				}
			case 'f':
				switch (second)
				{
				case ' ':
					return LineType::face;
				default:
					return LineType::unknown;
				}
			default:
				return LineType::unknown;
			}
		}

		struct SkipLine
		{
		};

		using ParsedLine =
			std::variant<SkipLine, Vertex, Normal, TexCoord, Face>;

		ParsedLine parse_line(Parser & parser)
		{
			ParsedLine result;

			switch (parse_line_type(parser))
			{
			case LineType::comment:
				result = SkipLine();
				break;

			// Halt processing immediately; don't skip afterwards
			case LineType::empty:
				return SkipLine();

			case LineType::vertex:
				result = parse_vertex(parser);
				break;

			case LineType::normal:
				result = parse_normal(parser);
				break;

			case LineType::tex_coord:
				result = parse_tex_coord(parser);
				break;

			case LineType::face:
			{
				Face face;

				for (auto it = face.points.begin(), itend = face.points.end();
					 it != itend; it++)
				{
					*it = parse_face_group(parser);
				}

				result = face;
				break;
			}
			default:
				result = SkipLine();
			}

			// Skip the rest of this line if not at EOF
			if (parser.more_input())
			{
				parser.ignore_until('\n');
			}

			return result;
		}
	}   // namespace

	ModelData parse_object_file(czstring filepath)
	{
		std::ifstream file(filepath);

		if (!file)
		{
			throw std::runtime_error(EXC_MSG("Couldn't open file for parsing"));
		}

		Parser parser(file);

		ModelData object;

		do
		{
			// Process a single line
			std::visit(
				overloaded{[](SkipLine) {},
						   [&object](Vertex vertex) {
							   object.vertex_data.points.push_back(vertex);
						   },
						   [&object](Normal normal) {
							   object.normal_data.points.push_back(normal);
						   },
						   [&object](TexCoord coord) {
							   object.uv_data.points.push_back(coord);
						   },
						   [&object](Face face) {
							   for (auto it = face.points.begin(),
										 itend = face.points.end();
									it != itend; it++)
							   {
								   object.vertex_data.indices.emplace_back(
									   it->vertex_index);
								   if (it->normal_index)
								   {
									   object.normal_data.indices.emplace_back(
										   *it->normal_index);
								   }
								   if (it->uv_index)
								   {
									   object.uv_data.indices.emplace_back(
										   *it->uv_index);
								   }
							   }
						   }},
				parse_line(parser));
		} while (!file.eof());

		return object;
	}
}   // namespace glge::obj_parser
