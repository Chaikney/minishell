---------
Minishell
---------

A simple Unix (i.e. only tested on Linux) shell written in C. Features input and output redirection, pipes, process handling and variable substitution to perform the basic things that you would expect from a shell, *and* it tries to behave in a more human-friendly way than the average shell.

Developed collaboratively in an educational context within a limited time. It takes ideas from lesser-used shells like `Fish <https://fishshell.com/>`_, and strives to do the most sensible thing rather than the aiming for quirk-for-quirk compatibility with bash.

..WARNING:: This is an education project. It would be reckless to use it for serious work, though I am pretty sure it can't do a *lot* of harm.

What I learned in this project
------------------------------

Technical things learned and demonstrated in this project include:

- UNIX **pipes**
- Input / output ***redirection**
- the **"fork and exec"** model of process handling.
- leak-free memory handling in processes and subprocesses
- environment variables: parsing, modifying and passing them to subprocesses.
- Design of shells
  For example the tradeoffs implicit in simple-seeming code decisions like the order of parsing variables and substitutions.
- GNU ***readline** library
- Memory safety debugging with **valgrind**
- Input Parsing.

  Hardening it against malevolent and accidental damage. The ways in which decisions (e.g. order of handling quotation types) have effects downstream (e.g. in the availability of variables to substitute at different times.)

- Input substitution behaviour known in bash as "here_doc"
  I still agree with Fish people that it is a weird quirk, but also some people find it very useful and not easily replaceable. We implemented it, while aavoiding the "bug-for-bug compatibility" trap.

So-called "soft skills"[#]_ demonstrated in this project.

- Project management.

  Keeping things within scope, prioritising tasks, etc

- Division of labour in a complex software project
- Use of **git** features (branches, merges, rebases) to resolve conflicts and work effectively in a team.
- The necessity for and limits of partitioning work packages in a complex software project.

  Specifically this project began with a strict split between execution and parsing modules. Later we found that the interface and data sharing between those two parts were tightly coupled. That required redesigns and iteration on ideas to find the best approach available to us.


Remaining limitations
---------------------

This is not a full-powered shell! Some hard problems were designed out by placing limitations and refusing to act in certain cases. For example, if an input line is too long, we simply refused to handle it. [#]_


Some features of larger shells were deemed out of scope, including: multiple redirection of output or input; variable substitution outside of normal input; more esoteric control structures such as ";" and "&&" ; wildcard "globbing" and script execution. I'd like to think that the code as structured could be extended to support the addition of these features without too much work.

The code bears the marks of having evolved in structure and purpose. While I tried to regularly refactor and rename functions, files, and variables, there may be some cases where the various parts could be more clearly explained. It may not serve you well as a model to follow.

The above also applies to the comment blocks. As part of collaborative working I tried to make sure that the documentation for each function was concise, clear and correct as the work continued. As the deadline approached, however, there may be changes to code where the documentation didn't keep pace. That was a learning experience and I guess realistic conditions. Not every project can be made to look perfect but it is important to keep the code readable as much as possible despite that. I think this project shows my ability to do that.

Conclusion
----------

During my work so far at `42 Urduliz <https://www.42urduliz.com/>`_ this may be the most challenging (yet rewarding) project I have tackled; that's why I am taking the time to document it here.

It shows that I can take my knowledge of C programming and apply it to a realistic software development project ending in a finished, usable product. One which has reasonably clear and well-documented code without traps or footguns and which does not leak memory or crash.

.. [#] Soft skills are hard!
.. [#] It is called *mini*shell, after all!
