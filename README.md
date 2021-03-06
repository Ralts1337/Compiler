# Compiler
Syntax Analysis part of a compiler

It prints out a AST in xml format based on user input and checks if there are any grammar errors.

For example:
1. user cannot use a variable before declaration (static binding).
2. user cannot bind value to a variable at the same time with declaration.
3. loops and logical decisions have to be closed properly

Test file: 
```sh
{
  int a ;
  a = 1 ;
  while ( a < 10 ) {
    a = a + 1 ; 
  }
} 
```
Gives the following AST:
```sh
<program>
 <block>
  <decls>
   <decl>
    <type>
     int
     <typeclosure>
     </typeclosure>
    </type>
    ;
   </decl>
   <decls>
   </decls>
  </decls>
  <stmts>
   <stmt>
    <loc>
     a
     <locclosure>
     </locclosure>
    </loc>
    =
    <bool>
     <join>
      <equality>
       <rel>
        <expr>
         <term>
          <unary>
           <factor>
            1
           </factor>
          </unary>
          <termclosure>
          </termclosure>
         </term>
         <exprclosure>
         </exprclosure>
        </expr>
        <reltail>
        </reltail>
       </rel>
       <equalityclosure>
       </equalityclosure>
      </equality>
      <joinclosure>
      </joinclosure>
     </join>
     <boolclosure>
     </boolclosure>
    </bool>
   </stmt>
   <stmts>
    <stmt>
     while
     <bool>
      <join>
       <equality>
        <rel>
         <expr>
          <term>
           <unary>
            <factor>
             <loc>
              a
              <locclosure>
              </locclosure>
             </loc>
            </factor>
           </unary>
           <termclosure>
           </termclosure>
          </term>
          <exprclosure>
          </exprclosure>
         </expr>
         <reltail>
          <
          <expr>
           <term>
            <unary>
             <factor>
              10
             </factor>
            </unary>
            <termclosure>
            </termclosure>
           </term>
           <exprclosure>
           </exprclosure>
          </expr>
         </reltail>
        </rel>
        <equalityclosure>
        </equalityclosure>
       </equality>
       <joinclosure>
       </joinclosure>
      </join>
      <boolclosure>
      </boolclosure>
     </bool>
     <stmt>
      <block>
       <decls>
       </decls>
       <stmts>
        <stmt>
         <loc>
          a
          <locclosure>
          </locclosure>
         </loc>
         =
         <bool>
          <join>
           <equality>
            <rel>
             <expr>
              <term>
               <unary>
                <factor>
                 <loc>
                  a
                  <locclosure>
                  </locclosure>
                 </loc>
                </factor>
               </unary>
               <termclosure>
               </termclosure>
              </term>
              <exprclosure>
               +
               <term>
                <unary>
                 <factor>
                  1
                 </factor>
                </unary>
                <termclosure>
                </termclosure>
               </term>
               <exprclosure>
               </exprclosure>
              </exprclosure>
             </expr>
             <reltail>
             </reltail>
            </rel>
            <equalityclosure>
            </equalityclosure>
           </equality>
           <joinclosure>
           </joinclosure>
          </join>
          <boolclosure>
          </boolclosure>
         </bool>
        </stmt>
        <stmts>
        </stmts>
       </stmts>
      </block>
     </stmt>
    </stmt>
    <stmts>
    </stmts>
   </stmts>
  </stmts>
 </block>
</program>
```
