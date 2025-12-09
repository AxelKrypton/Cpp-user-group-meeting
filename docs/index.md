---
hide:
 - toc
---

!!! danger "All good things come to an end!"

    Due to staff changes in the Z02 project of the CRC-TR 211 collaboration, this C++ user group meeting series has been sadly suspended till further notice at the end of 2025.

![CppLogo](images/CppLogo.png){ width="15%", align=right }

## The meetings format

> *«Just play. Have fun. Enjoy the game»* &dash; Michael Jordan.

The meetings aim at exploring and learning together aspects of C++.
The format is meant to be informal and the discussion open.
**Everybody is welcome.**
Feel free to bring your questions on the subject with you and to propose topics for future meetings as well.
The discussion will be ideally driven by questions and by participants.
Format adjustments might be done according to participants wishes.


## Important information

This series of meetings is offered in the context of the [CRC-TR 211](https://itp.uni-frankfurt.de/~strongmatter/) collaboration and, in particular, by the Z02 project.
Topics will be chosen on the way and according to attendees' needs.

 - **Day and time:** Every last Monday of the month, normally at 16:30
 - **Duration:** 90 minutes
 - **Target audience:** Any C++ programmer (also outside of the CRC)
 - **Level:** Preferably *basic to intermediate*, sometimes more advanced
 - **Moderator:** Alessandro Sciarra
 - **Why should I join?** There might be cookies :cookie:


## Meetings announcements

Every meeting will be announced roughly one week in advance through the CRC mailing list.

!!! info "A dedicated mailing list"

    If you are not on such a list and still wish to receive messages about the user group meetings, consider to register to [the dedicated mailing list of the meetings](https://lists.itp.uni-frankfurt.de/postorius/lists/cpp-ugm.lists.itp.uni-frankfurt.de/).
    You can do this as a guest without an account.
    In any case, you will need to first confirm the subscription yourself using a link that will be emailed to the registered address and, then, the administrator of the list will need to approve the subscription.
    Therefore, please, **prefer using an institutional email address** as suspect subscriptions will be rejected.


## Meetings snippet and material

In most of meetings we will code proof of concepts together.
The result of such a "group programming" will be uploaded into the repository underlying this website[^2].
This is not meant as Holy Grail but simply as starting point for further experimenting. :wink:

[^2]: You can reach it by clicking on the &nbsp;:simple-github: `Repository`&nbsp; button on the right of the bar at the top.


## Utilities

=== "The C++ language reference"

    Whenever you need to look up anything about C++ language, [this is definitely the best C++ reference](http://en.cppreference.com/w/).
    Almost each page contains examples and explanation.
    Sometimes this is very technical, but with some experience and complementing it with other resources (e.g. Stack Overflow) it is a great source to use.

=== "Compiler explorer"

    Do you want to play around with different compilers (or different compiler versions) or, maybe, inspect the assembler code?
    This is possible [in your browser](https://godbolt.org/) with a couple of clicks! :blush:
    And you can even get a link to share your experiment with whoever you want.
    Check out the following example!

    <div class="grid" markdown>

    ```c++
    #include <iostream>

    int main() {
      std::cout << "Hello world!\n";
    }
    ```

    [Try me live in Compiler Explorer!](https://godbolt.org/z/WThxrjMz9){ .md-button }

    </div>

=== "The C++ core guidelines"

    This is [an impressive project](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) undertaken by Bjarne Stroustrup (the inventor of C++) and Herb Sutter.
    It contains guidance about how to write good code and advice about what should be avoided.
    Get there from time to time and read around. :upside_down_face:

=== "The CppCon conference"

    This is [the main conference about C++](https://cppcon.org/).
    It contains sessions about *everything*.
    Some months after the conference many talks are uploaded [to youtube](https://www.youtube.com/@CppCon) and they offer a great resource from many perspectives.