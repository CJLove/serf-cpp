pipeline {
    agent none

	parameters {
        // With fir's upgrade to Fedora 31 and podman the Jenkins `docker` agent is no longer supported
        
        // Build on Fedora's default compiler as first parallel stages
        booleanParam name: 'Use_gcc9', defaultValue: true, description: 'Build/test using gcc9'
        booleanParam name: 'Use_clang9', defaultValue: true, description: 'Build/test using clang9'
        // Sanitizer tests in parallel stages
        booleanParam name: 'Run_sanitizers', defaultValue: false, description: 'Build/test using sanitizers'
        // Build alternative compilers in parallel stages
        booleanParam name: 'Use_gcc4', defaultValue: false, description: 'Build/test using gcc4'
        booleanParam name: 'Use_gcc5', defaultValue: false, description: 'Build/test using gcc5'
        booleanParam name: 'Use_gcc6', defaultValue: false, description: 'Build/test using gcc6'
        booleanParam name: 'Use_gcc7', defaultValue: false, description: 'Build/test using gcc7'
        booleanParam name: 'Use_gcc8', defaultValue: false, description: 'Build/test using gcc8'
	}
    stages {
        stage('Parallel system compiler stages') {
            failFast true
            parallel {
                stage('gcc9.3.1') {
                    when {
                        environment name: 'Use_gcc9', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        // Enable clang-tidy checks on this build and expect clean results
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 8.4.0"
                        dir ("gcc931") {
                            sh 'cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DENABLE_CLANG_TIDY=TRUE ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc931/unittests.xml'
                        }
                    }
                }
                stage('clang9') {
                    when {
                        environment name: 'Use_clang9', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        // Enable cppcheck on this build and expect clean results
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using clang 9"
                        dir ("clang9") {
                            sh 'CC=clang CXX=clang++ cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DENABLE_CPPCHECK=TRUE ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'clang9/unittests.xml'
                        }
                    }
                }
            }
        }

        stage('Parallel sanitizer stages') {
            failFast true
            parallel {
                stage('asan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("asan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=asan ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }

                stage('tsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("tsan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=tsan ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }

                stage('ubsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("ubsan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=ubsan ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }
            }
        }

        stage('Parallel alternate compiler stages') {
            failFast true
            parallel {
                stage('gcc8.4.0') {
                    when {
                        environment name: 'Use_gcc8', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 8.4.0"
                        dir ("gcc840") {
                            sh 'CC=/opt/gcc840/bin/gcc CXX=/opt/gcc840/bin/g++ cmake ..'
                            sh 'LD_LIBRARY_PATH=/opt/gcc840/lib64 make'
                            sh "LD_LIBRARY_PATH=/opt/gcc740/lib64 ./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc840/unittests.xml'
                        }
                    }
                }

                stage('gcc7.4.0') {
                    when {
                        environment name: 'Use_gcc7', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 7.4.0"
                        dir ("gcc740") {
                            sh 'CC=/opt/gcc740/bin/gcc CXX=/opt/gcc740/bin/g++ cmake ..'
                            sh 'LD_LIBRARY_PATH=/opt/gcc740/lib64 make'
                            sh "LD_LIBRARY_PATH=/opt/gcc740/lib64 ./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc740/unittests.xml'
                        }
                    }
                }

                stage('gcc6.5.0') {
                    when {
                        environment name: 'Use_gcc6', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 6.5.0"
                        dir ("gcc650") {
                            sh 'CC=/opt/gcc650/bin/gcc CXX=/opt/gcc650/bin/g++ cmake ..'
                            sh 'LD_LIBRARY_PATH=/opt/gcc650/lib64 make'
                            sh "LD_LIBRARY_PATH=/opt/gcc650/lib64 ./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc650/unittests.xml'
                        }
                    }
                }

                stage('gcc5.3.0') {
                    when {
                        environment name: 'Use_gcc5', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 5.3.0"
                        dir ("gcc530") {
                            sh 'CC=/usr/bin/gcc53 CXX=/usr/bin/g++53 cmake ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc530/unittests.xml'
                        }
                    }
                }

                stage('gcc4.9.3') {
                    when {
                        environment name: 'Use_gcc4', value: 'true'
                    }
                    agent {
                        node {
                            label 'fir'
                        }
                    }
                    steps {
                        echo "building serf-cpp branch ${env.BRANCH_NAME} using gcc 4.9.3"
                        dir ("gcc493") {
                            sh 'CC=/usr/bin/gcc49 CXX=/usr/bin/g++49 cmake ..'
                            sh 'make'
                            sh "./tests/SerfCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc493/unittests.xml'
                        }
                    }
                }
            }
        }
    }
    
}
