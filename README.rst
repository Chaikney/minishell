---------
Minishell
---------

A simple Unix (principally Linux, but should also work on Mac OS) shell written in C. Features input and output redirection, pipes, etc.

Developed in an educational context and aims for friendliness within the limits of time. It takes ideas from lesser-used shells like Fish, and strives to do the most sensible thing rather than the aiming for quirk-for-quirk compatibility with bash.

What I learned in this project
------------------------------

- Management and division of labour.
- Project management.
- Use of git features (branches, merges, rebases) to resolve conflicts and work effectively in a team.
- The necessity for and limits of partitioning work in a complex software project. Specifically this project began as split between execution and parsing modules. Later we found that the interface and data sharing between those two parts was more or less tightly coupled. That required redesigns and iteration on ideas to find the best approach we were able to use.
- UNIX pipes
- fork and exec model of process handling.
- GNU readline library
- memory handling in processes and subprocesses
- environment parsing, modifying and passing to subprocesses.
- Design of shells
- Parsing of input. Hardening it against malevolent and accidental damage. The ways in which decisions (e.g. order of handling quotation types) have effects downstream (e.g. in the availability of variables to substitute at different times.)
- bash's particular input substitution behaviour known as "here_doc". I still agree with Fish people that it is a weird quirk, but also some people find it very useful and not easily replaceable.

Remaining limitations
---------------------

This is not a full-powered shell! Some hard problems were avoided by placing limitations and refusing to act on certain cases. For example if the line is too long, we simply refused to handling it. (It is called "mini"shell, after all!)

The code bears the marks of having evolved in structure and purpose. While I tried to regularly refactor and rename functions, files, and variables, there may be some cases where the various parts could be more clearly explained. It may not serve you well as a model to follow.

The above also applies to the comment blocks. As part of collaborative working I tried to make sure that the documentation for each function was concise, clear and correct as the work continued. As the deadline approached, however, there may be changes to code where the documentation didn't keep pace. That was a learning experience and I guess realistic conditions. Not every project can be made to look perfect but it is important to keep the code readable as much as possible despite that. I think this project shows my ability to do that.

Conclusion
----------

During my work so far at 42 Urduliz this may be the most challenging yet rewarding project I have worked on; that's why I am taking the time to document it here.
It shows that I can apply the knowledge of languages to a realistic software development project which ends up with a finished, usable product.
